#include "InkBreakerConfig.h"

#include <iostream>

#include <stdlib.h>
#include <time.h>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_RGB_Image.H>


// #define STB_PERLIN_IMPLEMENTATION
// #include "stb/stb_perlin.h"
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb/stb_image.h"


#include "canvas.h"
//
// MAIN WINDOW CALLBACKS DECLARATIONS
//
void new_cb(Fl_Widget* widget, void*);
void save_cb(Fl_Widget* widget, void*);
void saveas_cb(Fl_Widget* widget, void*);
void quit_cb(Fl_Widget* widget, void*);
void about_cb(Fl_Widget* widget, void*);
//
// GENERATORS CALLBACKS DECLARATIONS
//
void blank_cb(Fl_Widget* widget, void*);
void line_splash_anim_cb(Fl_Widget* widget, void*);
void perlin_noise_anim_cb(Fl_Widget* widget, void*);
//
// MAIN WINDOW WIDGETS
//
Fl_Menu_Item menutable[] = {
    { "INKBREAKER", 0, (Fl_Callback*)about_cb, nullptr, FL_MENU_INACTIVE },
    { "&File",      0, nullptr, nullptr, FL_SUBMENU },
        { "&New",        FL_COMMAND + 'n', (Fl_Callback*)new_cb },
        { "&Save",       FL_COMMAND + 's', (Fl_Callback*)save_cb },
        { "&Save as...", FL_COMMAND + FL_ALT + 's', (Fl_Callback*)saveas_cb },
        { "Quit", FL_COMMAND + 'q', (Fl_Callback*)quit_cb },
    { 0 },
    { "Generators", 0, nullptr, nullptr, FL_SUBMENU },
        { "Blank", 0, (Fl_Callback*)blank_cb},
        { "Line Splash Animation", 0, (Fl_Callback*)perlin_noise_anim_cb },
        { "Perlin Noise Animation", 0, (Fl_Callback*)perlin_noise_anim_cb },
        { 0 },
    { "&Help", 0, nullptr, nullptr, FL_SUBMENU },
        { "&About", 0, (Fl_Callback*)about_cb },
        { 0 },
    { 0 }
};
//
// MAIN FUNCTION
//
constexpr int MAIN_WIN_W = 480;
constexpr int MAIN_WIN_H = 360;
constexpr int MENUBAR_W = MAIN_WIN_W;
constexpr int MENUBAR_H = 30;
constexpr int CANVAS_W = 480;
constexpr int CANVAS_H = 360;

static void canvas_animation_cb(void*);
constexpr double delta_time = 0.05;
Canvas* canvas;

int main(void)
{
    // window
    Fl_Double_Window* main_window = new Fl_Double_Window{ MAIN_WIN_W, MAIN_WIN_H + MENUBAR_H };
    // windows items
    Fl_Menu_Bar menubar{ 0, 0, MENUBAR_W, MENUBAR_H };
    menubar.menu(menutable);
    canvas = new Canvas{ 0, MENUBAR_H, MAIN_WIN_W,MAIN_WIN_H,
        CANVAS_W, CANVAS_H, main_window };
    // adding window items
    main_window->begin();
    main_window->add(menubar);
    main_window->add(canvas);
    main_window->end();
    main_window->resizable(canvas);
    
    main_window->show();
    Fl::add_timeout(delta_time, canvas_animation_cb);
    return Fl::run();
}

static void canvas_animation_cb(void *)
{
    canvas->canvas_drawing();   // if the offscreen exists, draw something
    Fl::repeat_timeout(delta_time, canvas_animation_cb);
}
//
//MAIN WINDOW CALLBACKS DEFINITIONS
//
void new_cb(Fl_Widget* widget, void*)
{
    std::cout << widget->label() << '\n';
}
void save_cb(Fl_Widget* widget, void*) { }
void saveas_cb(Fl_Widget* widget, void*) { }
void quit_cb(Fl_Widget* widget, void*)
{
    std::cout << "Bye Bye." << std::endl;
    exit(0);
}
void about_cb(Fl_Widget* widget, void*)
{
    std::cout << "INKBREAKER — "
              << "Version " << INKBREAKER_VERSION_MAJOR << "."
              << INKBREAKER_VERSION_MINOR << "."
              << INKBREAKER_VERSION_PATCH << std::endl;
}
//
// GENERATORS CALLBACKS DEFINITIONS
//
void blank_cb(Fl_Widget* widget, void*)
{
    
}
void line_splash_anim_cb(Fl_Widget* widget, void*)
{
    
}
void perlin_noise_anim_cb(Fl_Widget* widget, void*)
{
    
}