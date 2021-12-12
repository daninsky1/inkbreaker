#include <math.h>
#include "main_window.h"
#include "state.h"

#include "view2d.h"

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
    mwnd->v2d->state.select = Select::move;
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

void poly_state_cb(Fl_Widget* widget, void* mwv)
{
    MainWindow* mwnd = static_cast<MainWindow*>(mwv);
    mwnd->v2d->state.mode = Mode::draw;
    mwnd->v2d->state.draw = Draw::poly;
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
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "vectorf_id_1 INTEGER,"
    "vectorf_id_2 INTEGER,"
    "FOREIGN KEY(vectorf_id_1)"
    "REFERENCES vectorf(id),"
    "FOREIGN KEY(vectorf_id_2)"
    "REFERENCES vectorf(id));"

    "CREATE TABLE circle("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
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

void save_file(std::vector<Shape*> shapes)
{
    // https://www.sqlite.org/backup.html
    char* z_err_msg = 0;

    sqlite3 *in_mem_db;
    sqlite3 *file_db;

    int rc = 0;

    // In memory database
    rc = sqlite3_open(":memory:", &in_mem_db);
    if (rc != SQLITE_OK) {
        // LOG
        std::cerr << "Failed to Open: " << sqlite3_errmsg(in_mem_db) << '\n';
        std::cerr << "Error code: " << rc << '\n';
        sqlite3_free(z_err_msg);
        return;
    }

    // Initialize new file_db
    rc = sqlite3_open(glob_filename, &file_db);
    if (rc != SQLITE_OK) {
        // LOG
        std::cerr << "Failed to Open: " << sqlite3_errmsg(file_db) << '\n';
        std::cerr << "Error code: " << rc << '\n';
        sqlite3_free(z_err_msg);
        return;
    }

    // Execute schema
    if (sqlite3_exec(in_mem_db, glob_sql_shapes_schema, NULL, NULL, &z_err_msg) != SQLITE_OK) {
        std::cout << "SQL error: " << z_err_msg << '\n';
        sqlite3_free(z_err_msg);
    }

    // Insert statements
    constexpr int sql_stmt_sz = 1024;
    char sql_stmt[sql_stmt_sz];

    for (size_t i = 0; i < shapes.size(); ++i) {
        // NOTE(daniel): Naive approach to inserting shapes hardcoded, for now
        // we only have shapes with 2 nodes points

        // Insert nodes statement
        float node0x = shapes[i]->nodes[0].pos.x;   // Nodes pos alias
        float node0y = shapes[i]->nodes[0].pos.y;
        float node1x = shapes[i]->nodes[1].pos.x;
        float node1y = shapes[i]->nodes[1].pos.y;
        sprintf_s(sql_stmt, sql_stmt_sz,
            "INSERT INTO vectorf(x, y) VALUES(%f, %f);"
            "INSERT INTO vectorf(x, y) VALUES(%f, %f);",
            node0x, node0y, node1x, node1y);
            
        // Execute nodes statement
        if (sqlite3_exec(in_mem_db, sql_stmt, NULL, NULL, &z_err_msg) != SQLITE_OK) {
            std::cout << "SQL error: Inserting nodes: " << z_err_msg << '\n';
            sqlite3_free(z_err_msg);
        }
        else {
            std::cout << "SQL success: Insert nodes\n";
        }

        // Get nodes ids
        // Get nodes statement
        sqlite3_stmt *comp_stmt = NULL;
        strcpy_s(sql_stmt, sql_stmt_sz, "SELECT * FROM vectorf ORDER BY id DESC LIMIT 2;");
        rc = sqlite3_prepare_v2(in_mem_db, sql_stmt, -1, &comp_stmt, NULL);

        if (!comp_stmt) {
            std::cout << "SQL error: Retrieving nodes id: " << rc << '\n';
        }

        // Fetch ids, the order are inverted
        sqlite3_step(comp_stmt);
        int nid1 = sqlite3_column_int(comp_stmt, 0);
        sqlite3_step(comp_stmt);
        int nid0 = sqlite3_column_int(comp_stmt, 0);
        if (sqlite3_step(comp_stmt) != SQLITE_DONE) {
            printf("Something gone wrong, fetching nodes\n");
        }

        // Insert shape  statement
        std::cout << shapes[i]->type() << std::endl;
        if (shapes[i]->type() == "line") {
            sprintf_s(sql_stmt, sql_stmt_sz,
                "INSERT INTO line(vectorf_id_1, vectorf_id_2) VALUES(%d, %d);",
                nid0, nid1);

            if (sqlite3_exec(in_mem_db, sql_stmt, NULL, NULL, &z_err_msg) != SQLITE_OK) {
                std::cout << "SQL error: Inserting nodes" << z_err_msg << '\n';
                sqlite3_free(z_err_msg);
            }
            else
                std::cout << "SQL insert line executed successfully\n";
        }
        else if (shapes[i]->type() == "rect") {
            sql_stmt[0] = '\0';
            sprintf_s(sql_stmt, sql_stmt_sz, "INSERT INTO rect(vectorf_id_1, vectorf_id_2) VALUES(%d, %d);",
                nid0, nid1);

            if (sqlite3_exec(in_mem_db, sql_stmt, NULL, NULL, &z_err_msg) != SQLITE_OK) {
                std::cout << "SQL error: Inserting nodes" << z_err_msg << '\n';
                sqlite3_free(z_err_msg);
            }
            else
                std::cout << "SQL insert rect executed successfully\n";
        }
        else if (shapes[i]->type() == "circle") {
            sql_stmt[0] = '\0';
            sprintf_s(sql_stmt, sql_stmt_sz, "INSERT INTO circle(vectorf_id_1, vectorf_id_2) VALUES(%d, %d);",
                nid0, nid1);

            if (sqlite3_exec(in_mem_db, sql_stmt, NULL, NULL, &z_err_msg) != SQLITE_OK) {
                std::cout << "SQL error: Inserting nodes" << z_err_msg << '\n';
                sqlite3_free(z_err_msg);
            }
            else
                std::cout << "SQL insert circle executed successfully\n";
        }
    }

    sqlite3_backup *backup = sqlite3_backup_init(file_db, "main", in_mem_db, "main");
    if (backup) {
        sqlite3_backup_step(backup, -1);
        sqlite3_backup_finish(backup);
    }
    sqlite3_close(file_db);
    sqlite3_close(in_mem_db);
    glob_changed = 0;
}

void save_cb(Fl_Widget* widget, void* mwv)
{
    MainWindow *mwnd = static_cast<MainWindow*>(mwv);

    if (glob_filename[0] == '\0') {
        // No filename - get one!
        saveas_cb(widget, mwv);
        return;
    }
    else {
        printf("changed false\n");
        mwnd->changed(false);
        save_file(mwnd->v2d->shapes);
    }
}

int check_save_popup()
{
    // prompt the user if file has pending issues,
    // true safe to continue, false user will not continue;
    return fl_choice("The current file has not been saved.\n"
        "Would you like to save it now?",
        "Cancel", "Save", "Don't Save");
}

void new_cb(Fl_Widget* widget, void* mwv)
{
    MainWindow *mwnd = static_cast<MainWindow*>(mwv);

    if (mwnd->changed()) {
        int choice = check_save_popup();
        //printf("%d\n", choice);
        // TODO(daniel): Name this magic numbers CANCEL = 0 SAVE = 1 DONT_SAVE = 3
        if (choice == 0) return;
        else if (choice == 1) {
            save_cb(widget, mwv);
        }
    }
    else {
        // TODO(daniel): Maybe wrap glob_filename into InkbreakerState
        strcpy_s(glob_filename, sizeof(glob_filename), "");
        mwnd->changed(false);
    }
    mwnd->changed(false);
    mwnd->v2d->clear();
}

void load_file(std::vector<Shape*> &shapes)
{
    // https://www.sqlite.org/backup.html
    char* z_err_msg = 0;

    sqlite3 *file_db;

    int rc = 0;

    // Initialize new file_db
    rc = sqlite3_open(glob_filename, &file_db);
    if (rc != SQLITE_OK) {
        // LOG
        std::cerr << "Failed to Open: " << sqlite3_errmsg(file_db) << '\n';
        std::cerr << "Error code: " << rc << '\n';
        sqlite3_free(z_err_msg);
        return;
    }

    // sql select statement
    constexpr int sql_stmt_sz = 1024;
    char sql_stmt[sql_stmt_sz];


    sqlite3_stmt *comp_stmt = NULL;
    float n0x, n0y, n1x, n1y;

    // Get lines
    strcpy_s(sql_stmt, sql_stmt_sz,
        "SELECT v1.x, v1.y, v2.x, v2.y FROM line"
        " INNER JOIN vectorf AS v1 ON line.vectorf_id_1=v1.id"
        " INNER JOIN vectorf AS v2 ON line.vectorf_id_2=v2.id;"
        );
    rc = sqlite3_prepare_v2(file_db, sql_stmt, -1, &comp_stmt, NULL);

    if (!comp_stmt) {
        std::cout << "SQL error: Compiling statement: " << rc << '\n';
    }

    // NOTE(daniel): This can be error prone, better way is to store the error code
    // node_0 and node_1

    do {
        rc = sqlite3_step(comp_stmt);
        Shape *shape = new Line();
        n0x = static_cast<float>(sqlite3_column_double(comp_stmt, 0));
        n0y = static_cast<float>(sqlite3_column_double(comp_stmt, 1));
        n1x = static_cast<float>(sqlite3_column_double(comp_stmt, 2));
        n1y = static_cast<float>(sqlite3_column_double(comp_stmt, 3));

        Vector2f n0{n0x, n0y};
        Vector2f n1{n1x, n1y};

        shape->get_next_node(n0);
        shape->get_next_node(n1);
        shapes.push_back(shape);
    } while (rc != SQLITE_DONE);

    // Get rect
    strcpy_s(sql_stmt, sql_stmt_sz,
        "SELECT v1.x, v1.y, v2.x, v2.y FROM rect"
        " INNER JOIN vectorf AS v1 ON rect.vectorf_id_1=v1.id"
        " INNER JOIN vectorf AS v2 ON rect.vectorf_id_2=v2.id;"
        );
    rc = sqlite3_prepare_v2(file_db, sql_stmt, -1, &comp_stmt, NULL);

    if (!comp_stmt) {
        std::cout << "SQL error: Compiling statement: " << rc << '\n';
    }

    // NOTE(daniel): This can be error prone, better way is to store the error code
    // node_0 and node_1

    do {
        rc = sqlite3_step(comp_stmt);
        Shape *shape = new Rect();
        n0x = static_cast<float>(sqlite3_column_double(comp_stmt, 0));
        n0y = static_cast<float>(sqlite3_column_double(comp_stmt, 1));
        n1x = static_cast<float>(sqlite3_column_double(comp_stmt, 2));
        n1y = static_cast<float>(sqlite3_column_double(comp_stmt, 3));

        Vector2f n0{n0x, n0y};
        Vector2f n1{n1x, n1y};

        shape->get_next_node(n0);
        shape->get_next_node(n1);
        shapes.push_back(shape);
    } while (rc != SQLITE_DONE);

    // Get circle
    strcpy_s(sql_stmt, sql_stmt_sz,
        "SELECT v1.x, v1.y, v2.x, v2.y FROM circle"
        " INNER JOIN vectorf AS v1 ON circle.vectorf_id_1=v1.id"
        " INNER JOIN vectorf AS v2 ON circle.vectorf_id_2=v2.id;"
        );
    rc = sqlite3_prepare_v2(file_db, sql_stmt, -1, &comp_stmt, NULL);

    if (!comp_stmt) {
        std::cout << "SQL error: Compiling statement: " << rc << '\n';
    }

    // NOTE(daniel): This can be error prone, better way is to store the error code
    // node_0 and node_1

    do {
        rc = sqlite3_step(comp_stmt);
        Shape *shape = new Circle();
        n0x = static_cast<float>(sqlite3_column_double(comp_stmt, 0));
        n0y = static_cast<float>(sqlite3_column_double(comp_stmt, 1));
        n1x = static_cast<float>(sqlite3_column_double(comp_stmt, 2));
        n1y = static_cast<float>(sqlite3_column_double(comp_stmt, 3));

        Vector2f n0{n0x, n0y};
        Vector2f n1{n1x, n1y};

        shape->get_next_node(n0);
        shape->get_next_node(n1);
        shapes.push_back(shape);
    } while (rc != SQLITE_DONE);

    sqlite3_close(file_db);
}

void open_cb(Fl_Widget* widget, void* mwv)
{
    MainWindow *mwnd = static_cast<MainWindow*>(mwv);

    if (mwnd->changed()) {
        int choice = check_save_popup();
        //printf("%d\n", choice);
        // TODO(daniel): Name this magic numbers CANCEL = 0 SAVE = 1 DONT_SAVE = 3
        if (choice == 0) return;
        else if (choice == 1) {
            save_cb(widget, mwv);
            mwnd->changed(false);
        }
    }
    else {
        // TODO(daniel): Maybe wrap glob_filename into InkbreakerState
        strcpy_s(glob_filename, sizeof(glob_filename), "");
        mwnd->changed(false);
    }

    mwnd->v2d->clear();
    mwnd->changed(false);

    Fl_Native_File_Chooser fnfc;
    fnfc.title("Open file");
    fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
    if (fnfc.show()) return;
    strcpy_s(glob_filename, 2048, fnfc.filename());

    load_file(mwnd->v2d->shapes);
}

inline char* get_file_ext(char* filename, size_t sz)
{
    while (sz--) {
        if (filename[sz] == '.') return &filename[sz];
    }
    return NULL;
}

void saveas_cb(Fl_Widget* widget, void* mwv)
{
    MainWindow *mwnd = static_cast<MainWindow*>(mwv);

    Fl_Native_File_Chooser fnfc = { };
    fnfc.title("Save As");
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
                save_file(mwnd->v2d->shapes);
            }
        }
        save_file(mwnd->v2d->shapes);
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
        { "&Open",       FL_COMMAND + 'o', (Fl_Callback*)open_cb },
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
        { "Polygon", 0, (Fl_Callback*)poly_state_cb},
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

MainWindow::MainWindow(int v2d_w, int v2d_h) :
    Fl_Double_Window{ v2d_w, MENU_BAR_H + v2d_h, "InkBreaker" }
{
    // InkbreakerState
    app_state.changed = false;
    app_state.active_selection = nullptr;

    // View2D
    v2d = new View2D{ 0, MENU_BAR_H, v2d_w, v2d_h, shapes};
    v2d->app_state = &app_state;


    // Fl_Menu_Bar
    menu_bar = new Fl_Menu_Bar{ 0, 0, V2D_DEFAULT_W, MENU_BAR_H };
    menu_bar->menu(menu_items);
    resizable(v2d);

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

bool MainWindow::changed()
{
    app_state.changed = v2d->changed;
    return app_state.changed;
}

void MainWindow::changed(bool c)
{
    app_state.changed = c;
    v2d->changed = c;
}

