#include "InkBreakerConfig.h"

#include "main_window.h"

void new_cb(Fl_Widget* widget, void*);
void save_cb(Fl_Widget* widget, void*);
void saveas_cb(Fl_Widget* widget, void*);
void quit_cb(Fl_Widget* widget, void*);
void about_cb(Fl_Widget* widget, void*);
void clear_cb(Fl_Widget* widget, void*);
void line_state_cb(Fl_Widget* widget, void*);
void save_cb(Fl_Widget* widget, void*);
void saveas_cb(Fl_Widget* widget, void*);
void pan_state_cb(Fl_Widget* widget, void*);
void zoom_state_cb(Fl_Widget* widget, void*);
void controls_cb(Fl_Widget* widget, void*);

constexpr int MENUBAR_H = 30;

MainWindow::MainWindow(int sspw, int ssph, const char* l) :
    Fl_Double_Window{ sspw, MENUBAR_H+ssph, l },
    menubar{0, 0, sspw, 30},
    screensp{ new ScreenSpace{0, MENUBAR_H, sspw, ssph, this} }
{
    make_current();
    menutable = new Fl_Menu_Item[]{
        { "INKBREAKER", 0, nullptr, nullptr, FL_MENU_INACTIVE },
        { "&File",      0, nullptr, nullptr, FL_SUBMENU },
            { "Clear",       FL_COMMAND + 'n', (Fl_Callback*)clear_cb, this },
            { "&New",        FL_COMMAND + 'n', (Fl_Callback*)new_cb, this },
            { "&Save",       FL_COMMAND + 's', (Fl_Callback*)save_cb, this },
            { "&Save as...", FL_COMMAND + FL_ALT + 's', (Fl_Callback*)saveas_cb, this },
            { "Quit", FL_COMMAND + 'q', (Fl_Callback*)quit_cb, this },
        { 0 },
        // TODO: Implement procedural texture generation after deployment to image support
        /*
        { "Generators", 0, nullptr, nullptr, FL_SUBMENU },
            { "Blank", 0, (Fl_Callback*)blank_cb},
            { "Line Splash Animation", 0, (Fl_Callback*)perlin_noise_anim_cb },
            { "Perlin Noise Animation", 0, (Fl_Callback*)perlin_noise_anim_cb },
            { 0 },
        */
        { "State", 0, nullptr, nullptr, FL_SUBMENU },
            { "Pan", 0, (Fl_Callback*)pan_state_cb, this},
            { "Zoom", 0, (Fl_Callback*)zoom_state_cb, this},
            { "Draw", 0, nullptr, nullptr, FL_SUBMENU},
                { "Line", 0, (Fl_Callback*)line_state_cb, this},
                { 0 },
            { 0 },
        { "&Help", 0, nullptr, nullptr, FL_SUBMENU },
            { "Controls", 0, (Fl_Callback*)controls_cb, this },
            { "&About", 0, (Fl_Callback*)about_cb, this },
            { 0 },
        { 0 }
    };
    menubar.menu(menutable);
    add(menubar);
    add(screensp);
} // MainWindow

MainWindow::~MainWindow()
{
    delete screensp;
}

void pan_state_cb(Fl_Widget* widget, void*)
{
    Fl_Menu_Item* item = (Fl_Menu_Item*)widget;
    std::cout << item->submenu() << std::endl;
}
void zoom_state_cb(Fl_Widget* widget, void*)
{
}
void saveas_cb(Fl_Widget* widget, void*)
{
}
//
// CALLBACKS DEFINITIONS
//
void quit_cb(Fl_Widget* widget, void*)
{
    std::cout << "Bye Bye." << std::endl;
    exit(0);
}

void controls_cb(Fl_Widget* widget, void*)
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
    MainWindow* mw = static_cast<MainWindow*>(mwv);
    mw->screensp->clear();

}

void line_state_cb(Fl_Widget* widget, void*)
{
}

void new_cb(Fl_Widget* widget, void*)
{
    std::cout << "New\n";
}

void save_cb(Fl_Widget* widget, void*)
{
}
