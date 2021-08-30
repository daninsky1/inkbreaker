#include "InkBreakerConfig.h"

#include <iostream>
#include <clocale>
#include <sstream>

#include <stdlib.h>
#include <time.h>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_RGB_Image.H>

#include <sqlite/sqlite3.h>

#include "main_window.h"
#include "screen_space.h"


// #define STB_PERLIN_IMPLEMENTATION
// #include "stb/stb_perlin.h"
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb/stb_image.h"


#include "canvas.h"
constexpr int MAIN_WIN_W = 680;                 // main_window w
constexpr int MENUBAR_W = MAIN_WIN_W;
constexpr int MENUBAR_H = 30;
constexpr int SSPW = 680;                 // ScreenSpace w, h
constexpr int SSPH = 360;

ScreenSpace* screen_space;

int main(void)
{
    sqlite3* db;
    char* z_err_msg = 0;
    int rc;
    rc = sqlite3_open("my_db.db", &db);
    char* sql;

    // Open db
    if (rc) {
        std::cerr << "Failed to Open: " << sqlite3_errmsg(db) << '\n';
        return 0;
    }
    else
        std::cout << "Successfully Open\n";

    // Create a SQL statement
    sql = "CREATE TABLE shapes("
        "id INT PRIMARY KEY NOT NULL,"
        "name TEXT NOT NULL);";

    // Execute SQL
    rc = sqlite3_exec(db, sql, NULL, NULL, &z_err_msg);

    if (rc != SQLITE_OK) {
        std::cout << "SQL error: " << z_err_msg << '\n';
        sqlite3_free(z_err_msg);
    }
    else
        std::cout << "SQL executed successfully\n";

    // Close db
    sqlite3_close(db);

    // window
    MainWindow* main_win = new MainWindow{ SSPW, SSPH };
    //Fl_Double_Window* main_window = new Fl_Double_Window{ MAIN_WIN_W, MAIN_WIN_H };
    //// windows items
    //Fl_Menu_Bar menubar{ 0, 0, MENUBAR_W, MENUBAR_H };
    //menubar.menu(menutable);
    //screen_space = new ScreenSpace{ 0, MENUBAR_H, SPW, SPH, main_window };
    //// adding window items
    //main_window->begin();
    //main_window->add(menubar);
    //main_window->add(screen_space);
    //main_window->end();
    ////main_window->resizable(screen_space);
    //
    //main_window->show();
    main_win->show();
    Fl_Double_Window w{ 300, 300, "hi" };
    w.show();
    //Fl::add_timeout(delta_time, canvas_animation_cb);
    return Fl::run();
}
//
//MAIN WINDOW CALLBACKS DEFINITIONS
//
