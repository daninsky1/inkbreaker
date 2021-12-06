#pragma once

#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>

#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Image_Surface.H>
#include <FL/platform.H>
#include <FL/fl_draw.H>
#include <FL/fl_ask.h>
#include <FL/fl_show_colormap.H>
#include <FL/Enumerations.H>

#include "shapes.h"
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
// LEFT MOUSE BUTTON MODE
//
enum class Draw {
    line,
    rect,
    circle
};

enum class Mode {
    select,
    pan,
    zoom,
    draw
};

struct View2DState {
    Mode mode;
    Draw draw;
};
//
// THE SCREEN CONTEXT
//

class View2D : public Fl_Box {
public:
    View2D(int x, int y, int w, int h, Fl_Double_Window* wnd);

    void world_to_scr(Vector world, int &scrx, int &screeny);
    void scr_to_world(int scrx, int screeny, Vector& world);

    void draw();
    void draw_create_shape();

    int handle(int evt);
    void set_cursor();

    void pan();
    void zoom(int focusx, int focusy, float scale_factor_percent);

    // NOTE(daniel): The window that we are
    Fl_Double_Window* m_wnd;

    Fl_Surface_Device *this_surface_device;
    Fl_Image_Surface *dev_scr_buf;

    // TODO(daniel): To be implemented minimum widget size
    int min_w = 680;
    int min_h = 360;

    // NOTE(daniel): This widget position are relative to the window
    int ssx, ssy;
    int ssw, ssh;

    View2DState state{ Mode::draw, Draw::circle };

    // NOTE(daniel): World in relation to the screen
    Vector world_offset{ 0.0, 0.0 };
    float world_scale{ 1.0f };

    // NOTE(daniel): Mouse states, the screen coordinates are relative to this
    // widget
    Pointi m_mouse_scr_pos{ 0, 0 };
    Vector m_mouse_world_pos;
    Pointi snap_mouse_scr_pos{ 0, 0 };
    Vector snap_mouse_world_pos;

    // TODO(daniel): Make a structure with the mouse states
    bool m_drag_constraint = false;
    float drag_sx = 0, drag_sy = 0;// drag start position
    bool is_dragging = false;
    Mode m_lm_state{ Mode::zoom };

    std::string m_md_scr_msg{ "zoom" };
    float pixel_size = 1.0f;
    float grid_snap_interval = pixel_size;
    float visual_grid_interval;

    bool is_drawing = false;
    sShape* temp_shape{ nullptr };
	std::vector<sShape*> shapes;
    sNode* m_selected_node{ nullptr };
    ShapeInfo sinfo{ 1, FL_WHITE, FL_BLUE };

    float max_zoom = 300.0f;
    float min_zoom = 0.001f;
    // NOTE(daniel): Scrolling and keyboard zooming in and out factor and sensitivity, 1.0
    // correspont to 100%. Ultra imprecise
    float zooming_factor = 0.10f;  // Scroll zoom
    float zooming_sens   = 0.01f;  // Drag per pixel zoom 
private:
    Fl_Offscreen scr_buf;        // Canvas buffer
    // NOTE(daniel): This is a FLTK offscreen implementation detail, I think is
    // to set the DPI factor
    float fl_offscr_scale = 1.0f;
};
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
    Mode m_mode_state = Mode::select;


};

