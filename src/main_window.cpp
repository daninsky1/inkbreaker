#define INKBREAKER_VERSION_MAJOR 0
#define INKBREAKER_VERSION_MINOR 0
#define INKBREAKER_VERSION_PATCH 1

#include <math.h>
#include "main_window.h"

#include "view2d.h"

void save_cb(Fl_Widget* widget, void*)
{
}

void controls_cd(Fl_Widget* widget, void*)
{
    fl_message("change left mouse button mode:\n"
        "pan: h or space or middle mouse button directly\n"
        "zoom: z\n"
        "nothing: 0");
}

void about_cb(Fl_Widget* widget, void*)
{
    std::cout << "INKBREAKER - "
        << "Version " << INKBREAKER_VERSION_MAJOR << "."
        << INKBREAKER_VERSION_MINOR << "."
        << INKBREAKER_VERSION_PATCH << std::endl;
}

void clear_cb(Fl_Widget* widget, void* mwv)
{
    MainWindow* mwnd = static_cast<MainWindow*>(mwv);
}

void select_state_cb(Fl_Widget* widget, void* mwv)
{
    MainWindow *mwnd = static_cast<MainWindow*>(mwv);
    mwnd->v2d->state.mode = Mode::select;
}

void pan_state_cb(Fl_Widget* widget, void* mwv)
{
    MainWindow *mwnd = static_cast<MainWindow*>(mwv);
    mwnd->v2d->state.mode = Mode::pan;
}

void zoom_state_cb(Fl_Widget* widget, void* mwv)
{
    MainWindow *mwnd = static_cast<MainWindow*>(mwv);
    mwnd->v2d->state.mode = Mode::zoom;
}

void line_state_cb(Fl_Widget* widget, void* mwv)
{
    MainWindow* mwnd = static_cast<MainWindow*>(mwv);
    mwnd->v2d->state.mode = Mode::draw;
    mwnd->v2d->state.draw = Draw::line;
}
void rect_state_cb(Fl_Widget* widget, void* mwv)
{
    MainWindow* mwnd = static_cast<MainWindow*>(mwv);
    mwnd->v2d->state.mode = Mode::draw;
    mwnd->v2d->state.draw = Draw::rect;
}
void circle_state_cb(Fl_Widget* widget, void* mwv)
{
    MainWindow* mwnd = static_cast<MainWindow*>(mwv);
    mwnd->v2d->state.mode = Mode::draw;
    mwnd->v2d->state.draw = Draw::circle;
}

void set_line_color_cb(Fl_Widget* widget, void* mwv)
{
    MainWindow* mwnd = static_cast<MainWindow*>(mwv);
    Fl_Color oldcl = mwnd->v2d->sinfo.line_color;
    Fl_Color c = fl_show_colormap(FL_BLACK);
    mwnd->v2d->sinfo.line_color = c;
}
void set_fill_color_cb(Fl_Widget* widget, void* mwv)
{
    MainWindow* mwnd = static_cast<MainWindow*>(mwv);
    Fl_Color oldcl = mwnd->v2d->sinfo.fill_color;
    Fl_Color c = fl_show_colormap(FL_BLACK);
    mwnd->v2d->sinfo.fill_color = c;
}
//
// GLOBALS
//
static bool glob_changed = false;
static char glob_filename[FL_PATH_MAX] = "";
static char glob_title[FL_PATH_MAX];
static sqlite3 *glob_db;

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


//void draw_msg(char *msg, float wsxf, float wsyf, int fnt_sz, float scale)
//{
//	fl_font(FL_TIMES_BOLD_ITALIC, static_cast<Fl_Fontsize>(fnt_sz*scale));
//
//    // world space text float
//    // screen space text int
//	int ssxi, ssyi;
//	world_to_scr(wsxf, wsxf, ssxi, ssyi);
//	fl_draw(msg, ssxi, ssyi);
//}

Fl_Menu_Item menu_items[] = {
    { "INKBREAKER", 0, (Fl_Callback*)about_cb, nullptr, FL_MENU_INACTIVE },
    { "&File",      0, nullptr, nullptr, FL_SUBMENU },
        { "&New",        FL_COMMAND + 'n', (Fl_Callback*)new_cb },
        { "&Save",       FL_COMMAND + 's', (Fl_Callback*)save_cb },
        { "&Save as...", FL_COMMAND + FL_ALT + 's', (Fl_Callback*)saveas_cb },
        { "Quit", FL_COMMAND + 'q', (Fl_Callback*)quit_cb },
        { 0 },
    // TODO: Implement procedural texture generation after deployment to image support
    // called InkBlend
    /*
    { "InkBlend", 0, nullptr, nullptr, FL_SUBMENU },
        { "Blank", 0, (Fl_Callback*)blank_cb},
        { "Line Splash Animation", 0, (Fl_Callback*)perlin_noise_anim_cb },
        { "Perlin Noise Animation", 0, (Fl_Callback*)perlin_noise_anim_cb },
        { 0 },
    */
    { "Mode", 0, nullptr, nullptr, FL_SUBMENU },
        { "Select", 0, (Fl_Callback*)select_state_cb},
        { "Pan", 0, (Fl_Callback*)pan_state_cb},
        { "Zoom", 0, (Fl_Callback*)zoom_state_cb},
        { 0 },
    { "View", 0, nullptr, nullptr, FL_SUBMENU },
        { "Grid", 0, (Fl_Callback*)pan_state_cb},
        { 0 },
    { "Draw", 0, nullptr, nullptr, FL_SUBMENU },
        { "Line", 0, (Fl_Callback*)line_state_cb},
        { "Rectangle", 0, (Fl_Callback*)rect_state_cb},
        { "Circle", 0, (Fl_Callback*)circle_state_cb},
        { 0 },
    { "Shape Style", 0, nullptr, nullptr, FL_SUBMENU },
    //    { "Line Width", 0, nullptr, nullptr, FL_SUBMENU },
    //        { "1", 0, (Fl_Callback*)set_line_width_cb },
    //        { "2", 0, (Fl_Callback*)set_line_width_cb },
    //        { "3", 0, (Fl_Callback*)set_line_width_cb },
    //        { "4", 0, (Fl_Callback*)set_line_width_cb },
    //        { "5", 0, (Fl_Callback*)set_line_width_cb },
    //        { 0 },
        { "Line Color", 0, (Fl_Callback*)set_line_color_cb },
        { "Fill Color", 0, (Fl_Callback*)set_fill_color_cb },
        { 0 },
    { "&Help", 0, nullptr, nullptr, FL_SUBMENU },
        { "Controls", 0, (Fl_Callback*)controls_cd },
        { "&About", 0, (Fl_Callback*)about_cb },
        { 0 },
    { 0 }
};

MainWindow::MainWindow() :
    Fl_Double_Window{ V2D_DEFAULT_W, MENU_BAR_H + V2D_DEFAULT_H, "MainWindow" }
{
    // IMPORTANT(daniel): Passing this pointer to v2d is really shady. For
    // reasons that I not fully understand. Constructing the v2d after menu bar
    // causes read violation
    v2d = new View2D{0, MENU_BAR_H, V2D_DEFAULT_W, V2D_DEFAULT_H + MENU_BAR_H, this};

    for (int i = 0; i < menu_items->size(); ++i) {
        if (menu_items[i].label()) {
            menu_items[i].user_data(this);
        }
    }
    menu_bar = new Fl_Menu_Bar{0, 0, V2D_DEFAULT_W, MENU_BAR_H};
    menu_bar->menu(menu_items);
    resizable(v2d);
    menu_bar->redraw();
}

MainWindow::MainWindow(int w, int h, const char* l) :
    Fl_Double_Window{ w, MENU_BAR_H + h, l }
{
    v2d = new View2D{0, MENU_BAR_H, V2D_DEFAULT_W, V2D_DEFAULT_H + MENU_BAR_H, this};
    
    menu_bar = new Fl_Menu_Bar{0, 0, V2D_DEFAULT_W, MENU_BAR_H};
    menu_bar->menu(menu_items);
    resizable(v2d);
    menu_bar->redraw();

    // IMPORTANT(daniel): This can problematic, menu_bar doesn't allow to change
    // the state of its menu_items, so const_cast was used, until I find a better
    // way to pass this object to the menu items callback data
    for (int i = 0; i < menu_bar->size(); ++i) {
        if (menu_bar->menu()[i].label()) {
            Fl_Menu_Item *item = const_cast<Fl_Menu_Item*>(&menu_bar->menu()[i]);
            item->user_data(this);
        }
    }
}


