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

static int escape_handler(int evt)
{
    // NOTE: [...] FLTK has a 'global event handler' that makes Escape
    // try to close the window [...]:
    // https://www.fltk.org/doc-1.4/FAQ.html
    // To prevent any not handled escape key to close our main window we eat it
    if (evt == FL_SHORTCUT && Fl::event_key() == FL_Escape) return 1;
    return 0;
}

int main()
{
    // window
    MainWindow* main_wnd = new MainWindow{ V2D_DEFAULT_W, V2D_DEFAULT_H };
    main_wnd->show();
    Fl::add_handler(escape_handler);

    return Fl::run();
}
