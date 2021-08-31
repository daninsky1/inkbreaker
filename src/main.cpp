#include "InkBreakerConfig.h"

#include <iostream>
#include <clocale>
#include <sstream>

#include <stdlib.h>
#include <time.h>

#include <FL/Fl.H>


#include <sqlite/sqlite3.h>

#include "main_window.h"
#include "screen_space.h"


// #define STB_PERLIN_IMPLEMENTATION
// #include "stb/stb_perlin.h"
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb/stb_image.h"


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
    main_win->show();

    return Fl::run();
}
