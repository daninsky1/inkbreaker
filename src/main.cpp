#include <iostream>
#include <clocale>
#include <sstream>

#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <FL/Fl.H>
#include <FL/filename.H>
#include <FL/Fl_Native_File_Chooser.H>

#include <sqlite/sqlite3.h>

#include "main_window.h"


// #define STB_PERLIN_IMPLEMENTATION
// #include "stb/stb_perlin.h"
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb/stb_image.h"

//
// GLOBALS
//
static bool glob_changed = false;
static char glob_filename[FL_PATH_MAX] = "";
static char glob_title[FL_PATH_MAX];
static sqlite3* glob_db;

// TODO(daniel): create a data structure to save
/*
CREATE TABLE vectorf(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    x FLOAT,
    y FLOAT);

CREATE TABLE line(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    vectorf_id_1 INTEGER,
    vectorf_id_2 INTEGER,
    FOREIGN KEY(vectorf_id_1)
    REFERENCES vectorf(id),
    FOREIGN KEY(vectorf_id_2)
    REFERENCES vectorf(id));

CREATE TABLE rect(
    id INTEGER PRIMARY KEY NOT NULL,
    vectorf_id_1 INTEGER,
    FOREIGN KEY(vectorf_id_1)
    REFERENCES vectorf(id),
    vectorf_id_2 INTEGER,
    FOREIGN KEY(vectorf_id_2)
    REFERENCES vectorf(id));
*/

static char* glob_sql_shapes_schema = "CREATE TABLE vectorf("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "x FLOAT,"
    "y FLOAT);"

    "CREATE TABLE line("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "vectorf_id_1 INTEGER,"
    "vectorf_id_2 INTEGER,"
    "FOREIGN KEY(vectorf_id_1)"
    "REFERENCES vectorf(id),"
    "FOREIGN KEY(vectorf_id_2)"
    "REFERENCES vectorf(id));"

    "CREATE TABLE rect("
    "id INT PRIMARY KEY NOT NULL,"
    "vectorf_id_1 INTEGER,"
    "vectorf_id_2 INTEGER,"
    "FOREIGN KEY(vectorf_id_1)"
    "REFERENCES vectorf(id),"
    "FOREIGN KEY(vectorf_id_2)"
    "REFERENCES vectorf(id));"

    "CREATE TABLE circle("
    "id INT PRIMARY KEY NOT NULL,"
    "vectorf_id_1 INTEGER,"
    "vectorf_id_2 INTEGER,"
    "FOREIGN KEY(vectorf_id_1)"
    "REFERENCES vectorf(id),"
    "FOREIGN KEY(vectorf_id_2)"
    "REFERENCES vectorf(id));";


View2D *view2d;

int get_node_id(void*, int, char**, char**)
{
    return 0;
}

void save_file()
{
    char* z_err_msg = 0;

    // Initialize new glob_db
    if (sqlite3_open(glob_filename, &glob_db) != SQLITE_OK) {
        // LOG
        std::cerr << "Failed to Open: " << sqlite3_errmsg(glob_db) << '\n';
        return;
    }
    else {
        // LOG success
    }


    if (sqlite3_exec(glob_db, glob_sql_shapes_schema, NULL, NULL, &z_err_msg) != SQLITE_OK) {
        std::cout << "SQL error: " << z_err_msg << '\n';
        sqlite3_free(z_err_msg);
    }
    else
        std::cout << "SQL new script executed successfully\n";

    // Add shapes
    constexpr int sql_insert_sz = 256;
    char sql_insert[sql_insert_sz];

    std::vector<int> shape_nodes_ids;
    for (size_t i = 0; i < m_shapes.size(); ++i) {
        // NOTE(daniel): insert nodes

        for (size_t j = 0; j < m_shapes[i]->max_nodes; ++j) {
            int node_id = (int)i*10 + (int)j;

            // NOTE(daniel): insert node statement
            shape_nodes_ids.push_back(node_id);
            sprintf_s(sql_insert, sql_insert_sz, "INSERT INTO vectorf(id, x, y) VALUES(%d, %f, %f);",
                node_id, m_shapes[i]->nodes[j].pos.x, m_shapes[i]->nodes[j].pos.y);

            // NOTE(daniel): execute node statement
            if (sqlite3_exec(glob_db, sql_insert, NULL, NULL, &z_err_msg) != SQLITE_OK) {
                std::cout << "SQL error: Inserting nodes" << z_err_msg << '\n';
                sqlite3_free(z_err_msg);
            }
            else
                std::cout << "SQL insert nodes executed successfully\n";
        }

        // TODO(daniel): insert shape
        if (m_shapes[i]->type().c_str() == "line") {
            sql_insert[0] = '\0';
            sprintf_s(sql_insert, sql_insert_sz, "INSERT INTO line(vectorf_id_1, vectorf_id_1) VALUES(%d, %d);",
                shape_nodes_ids[0], shape_nodes_ids[1]);

            if (sqlite3_exec(glob_db, sql_insert, NULL, NULL, &z_err_msg) != SQLITE_OK) {
                std::cout << "SQL error: Inserting nodes" << z_err_msg << '\n';
                sqlite3_free(z_err_msg);
            }
            else
                std::cout << "SQL insert line executed successfully\n";
        }
        else if (m_shapes[i]->type().c_str() == "rect") {
            sql_insert[0] = '\0';
            sprintf_s(sql_insert, sql_insert_sz, "INSERT INTO rect(vectorf_id_1, vectorf_id_1) VALUES(%d, %d);",
                shape_nodes_ids[0], shape_nodes_ids[1]);

            if (sqlite3_exec(glob_db, sql_insert, NULL, NULL, &z_err_msg) != SQLITE_OK) {
                std::cout << "SQL error: Inserting nodes" << z_err_msg << '\n';
                sqlite3_free(z_err_msg);
            }
            else
                std::cout << "SQL insert rect executed successfully\n";
        }

        shape_nodes_ids.clear();
    }

    glob_changed = 0;
}

void save_cb()
{
    if (glob_filename[0] == '\0') {
        // No filename - get one!
        saveas_cb();
        return;
    }
    else save_file();
}

bool check_save()
{
    // prompt the user if file has pending issues
    // 1 safe to continue, 0 user will not continue;
    if (!glob_changed) return true;

    else {
        int c = fl_choice("The current file has not been saved.\n"
            "Would you like to save it now?",
            "Cancel", "Save", "Don't Save");
        if (c == 1) {
            save_cb();
            return true;
        }
        return (c == 2) ? 1 : 0;
    }
}

void new_cb(Fl_Widget* widget, void* mwv)
{
    std::cout << "New\n";

    MainWindow *mwnd = static_cast<MainWindow*>(mwv);
    mwnd->v2d->shapes.clear();


    if (!check_save()) return;

    strcpy_s(glob_filename, sizeof(glob_filename),"");
    glob_changed = false;
}

char* get_file_ext(char* filename, size_t sz)
{
    while (sz--) {
        if (filename[sz] == '.') return &filename[sz];
    }
    return NULL;
}

void saveas_cb() {
    Fl_Native_File_Chooser fnfc;
    fnfc.title("Save File As?");
    fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
    fnfc.filter("sqlite3\t*.{sqlite3,sqlite,db}");
    fnfc.options(Fl_Native_File_Chooser::Option::USE_FILTER_EXT);
    fnfc.directory("./");

    int r = fnfc.show();

    if (r == -1) {
        std::cout << "ERROR: " << fnfc.errmsg() << '\n';
    }
    else if (r == 1) {
        std::cout << "CANCEL\n";
        return;
    }
    else {
        strcpy_s(glob_filename, FL_PATH_MAX, fnfc.filename());
        int ext_val = fnfc.filter_value();
        char* fileext = get_file_ext(glob_filename, strlen(glob_filename));

        if (ext_val == 0) {
            if (!fileext || (strcmp(fileext, ".sqlite3") && strcmp(fileext, ".sqlite") && strcmp(fileext, ".db"))) {
                strcat_s(glob_filename, sizeof(glob_filename), ".sqlite3");
                save_file();
            }
        }
        else {
            save_file();
        }

        std::cout << "FILE_NAME: " << glob_filename << '\n';
    }
}

void quit_cb(Fl_Widget* widget, void*)
{
    std::cout << "Bye Bye." << std::endl;
    exit(0);
}


int main(void)
{
    // window
    MainWindow* main_wnd = new MainWindow{ };
    main_wnd->show();

    return Fl::run();
}
