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
#include <FL/Enumerations.H>

#include "shapes.h"
//
// GLOBALS
//
static std::vector<sShape*> m_shapes;
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
    rect
};

enum class Mode {
    select,
    pan,
    zoom,
    draw
};

struct CanvaState {
    Mode mode;
    Draw draw;
};
//
// THE SCREEN CONTEXT
//

class ScreenSpace : public Fl_Box {
public:
    ScreenSpace(int x, int y, int w, int h, Fl_Double_Window* wnd);

    void world_to_scr(Vector world, int &scrx, int &screeny);

    void scr_to_world(int scrx, int screeny, Vector& world);

    void draw();
    void draw_create_shape();

    int handle(int evt);
    void set_cursor();
    void pan();     // change offset values
    void zoom();    // change scale factor

    // The window that ScreenSpace are
    Fl_Double_Window* m_wnd;

    Fl_Surface_Device *this_surface_device;
    Fl_Image_Surface *dev_scr_buf;

    int min_w = 680;    // Minimum widget size
    int min_h = 360;
    

    // This widget position and size relative to the window
    int ssx, ssy;
    int ssw, ssh;
    int m_sspw, m_ssph;                  // ScreanSpace size
    float m_buffer_scale{ 1.0f };        // m_screen_buffer scale, this is an FLTK implementation detail

    Vector m_off{ 0.0, 0.0 };            // world offset
    float m_scale{ 50.0f };               // world scale
    float m_scale_sens{ 1.1f };          // scale sensitivite
    Point_i m_mouse_scr_pos{ 0, 0 };     // mouse position in the widget screen
    Vector m_mouse_world_pos;            // mouse position in the world
    Vector m_mouse_world_snap_pos;       // mouse position in the world on the nearest snap point
    bool m_drag_constraint{ false };
    float m_drag_sx{ 0 }, m_drag_sy{ 0 };// drag start position
    bool m_drag_state{ true };
    Mode m_lm_state{ Mode::zoom };       // left mouse button state
    std::string m_md_scr_msg{ "zoom" };
    float pixel_size = 1.0f;
    float visual_grid_interval;
    float grid_snap_interval = pixel_size;

    sShape* m_temp_shape{ nullptr };
    sLine* m_line{ nullptr };
    sNode* m_selected_node{ nullptr };

    // Canva main state sets the main canva scene manipulation mode(selecting, drawing, zooming, etc)
    CanvaState cv_main_state{Mode::select, Draw::line};

    float max_zoom = 300.0f;          // Max Zooming
    float min_zoom = 0.001f;            // Min zooming
    // Scrolling and keyboard zooming in and out factor, 1.0 correspont to 100%
    // NOTE(daniel): Ultra imprecise
    float zooming_factor = 0.10f;
private:
    Fl_Offscreen scr_buf;        // Canvas buffer
    float scr_buf_scale;        // offscreen scale dpi thing
};
//
// MAIN WINDOW
//
class MainWindow : public Fl_Double_Window {
public:
    MainWindow(int sspw, int ssph, const char* l = "");
	void set_mode();
	void set_draw_state();

	/* Widgets */
    Fl_Menu_Bar menu_bar;
	ScreenSpace* canvas;

	/* States*/
    Mode m_mode_state = Mode::select;

	std::vector<sShape*> m_shapes;

};

