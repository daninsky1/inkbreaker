#include "InkBreakerConfig.h"

#include "main_window.h"

void new_cb(Fl_Widget* widget, void*);
void save_cb(Fl_Widget* widget, void*);
void saveas_cb(Fl_Widget* widget, void*);
void quit_cb(Fl_Widget* widget, void*);
void about_cb(Fl_Widget* widget, void*);

void blank_cb(Fl_Widget* widget, void*);
void line_splash_anim_cb(Fl_Widget* widget, void*);
void perlin_noise_anim_cb(Fl_Widget* widget, void*);
void line_state_cb(Fl_Widget* widget, void*) {}


void new_cb(Fl_Widget* widget, void*)
{
    std::cout << "New\n";
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
    std::cout << "INKBREAKER - "
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
    { "Draw", 0, nullptr, nullptr, FL_SUBMENU },
        { "Line", 0, (Fl_Callback*)line_state_cb},
        { 0 },
    { "&Help", 0, nullptr, nullptr, FL_SUBMENU },
        { "&About", 0, (Fl_Callback*)about_cb },
        { 0 },
    { 0 }
};


constexpr int MENUBAR_H = 30;

MainWindow::MainWindow(int sspw, int ssph, const char* l) :
    Fl_Double_Window{ sspw, MENUBAR_H+ssph, l },
    m_mbar{0, 0, sspw, 30},
    m_ssp{ new ScreenSpace{0, MENUBAR_H, sspw, ssph, this} }
{
    make_current();
    m_mbar.menu(menutable);
    add(m_mbar);
    add(m_ssp);
} // MainWindow

MainWindow::~MainWindow()
{
    delete m_ssp;
}

