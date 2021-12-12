#include <iostream>
#include <clocale>
#include <sstream>

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "main_window.h"

// #define STB_PERLIN_IMPLEMENTATION
// #include "stb/stb_perlin.h"
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb/stb_image.h"

int main()
{
    // window
    MainWindow* main_wnd = new MainWindow{ V2D_DEFAULT_W, V2D_DEFAULT_H };
    main_wnd->show();

    return Fl::run();
}
