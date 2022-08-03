#pragma once

#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>

#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>

#include <FL/fl_ask.H>
#include <FL/fl_show_colormap.H>
#include <FL/Fl.H>
#include <FL/filename.H>
#include <FL/Fl_Native_File_Chooser.H>

#include "objects/shapes.h"
#include "state.h"

#ifdef IO_SQLITE
#include <sqlite/sqlite3.h>
#endif

// #include "tree.h"

constexpr int V2D_DEFAULT_W = 680;
constexpr int V2D_DEFAULT_H = 360;
constexpr int MENU_BAR_H = 26;

// TODO(daniel): The main window structure needs to be better thought
struct MainWindowDefaultLayout {
    int v2dw = V2D_DEFAULT_W;
    int v2dh = V2D_DEFAULT_H;
    int menu_bar_h = MENU_BAR_H;
};
//
// CALLBACKS
//
void new_cb(Fl_Widget* widget, void *mwv);
void save_cb(Fl_Widget* widget, void *mwv);
void saveas_cb(Fl_Widget* widget, void *mwv);
void quit_cb(Fl_Widget* widget, void *mwv);

void about_cb(Fl_Widget* widget, void*);
void clear_cb(Fl_Widget* widget, void*);
//
// MAIN WINDOW
//
class View2D;
class MainWindow : public Fl_Double_Window {
public:
    MainWindow(int v2d_w, int v2d_h);

    void set_menu_items_pointer();
	void set_mode();
	void set_draw_state();

    bool changed();
    void changed(bool c);

    /* Shapes */
    // Tree::Node *root = new Tree::Node();
    std::vector<old::Shape*> shapes;
    old::Shape* active_selection;
    old::ShapeInfo shape_info{ 1, FL_BLACK, FL_BLUE };

	/* Widgets */
    Fl_Menu_Bar *menu_bar;
	View2D *v2d;

	/* States*/
    InkbreakerState state = { };
};
