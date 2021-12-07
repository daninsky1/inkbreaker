#pragma once

#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>

#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/fl_ask.h>
#include <FL/fl_show_colormap.H>

#include "view2d.h"
//
// GLOBALS
//
static std::vector<sShape*> m_shapes;

constexpr int V2D_DEFAULT_W = 680;
constexpr int V2D_DEFAULT_H = 360;
constexpr int MENU_BAR_H = 26;

// TODO(daniel): The main window structure needs to be better thought
struct MainWindowDefaultLayout {
    int v2dw = V2D_DEFAULT_W;
    int v2dh = V2D_DEFAULT_H;
    int menu_bar_h = MENU_BAR_H;
};

struct InkbreakerState {
    bool changed;
};
//
// CALLBACKS
//
void new_cb(Fl_Widget* widget, void*);
void save_cb();
void saveas_cb();
void quit_cb(Fl_Widget* widget, void*);
void about_cb(Fl_Widget* widget, void*);
void clear_cb(Fl_Widget* widget, void*);



//
// MAIN WINDOW
//
class MainWindow : public Fl_Double_Window {
public:
    MainWindow();
    MainWindow(int sspw, int ssph, const char* l = "");

    void set_menu_items_pointer();
	void set_mode();
	void set_draw_state();

	/* Widgets */
    Fl_Menu_Bar *menu_bar;
	View2D *v2d;

	/* States*/
    InkbreakerState *main_state;
    Mode m_mode_state = Mode::select;
};

