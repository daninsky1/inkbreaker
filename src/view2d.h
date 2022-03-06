#pragma once

#include <assert.h>

#include <sstream>

#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Image_Surface.H>
#include <FL/platform.H>
#include <FL/fl_draw.H>
#include <FL/Enumerations.H>

#include "shapes.h"
#include "state.h"
//
// VIEW 2D STATE
//
enum class Mode {
    select,
    pan,
    zoom,
    draw
};

enum class Draw {
    line,
    rect,
    circle,
    poly,
    bezier
};

enum class Select {
    move,
    scale,
    rotate
};


struct View2DState {
    Mode mode;

    Select select;
    Draw draw;
    bool is_grid_snap;
};
//
// THE SCREEN CONTEXT
//
static void draw_sine_wave()
{
    // NOTE(daniel): Sine wave
    // TODO(daniel): Wrap this to a function
	//fl_color(FL_WHITE);
	//fl_line_style(FL_SOLID, 2*(int)world_scale);
	//fl_begin_line();
	//for (float i = 0; i < w(); i += 0.1f) {
	//	Vector2f wv{ i, std::sin(i * (0.5f * 0.1f)) * 50 };	// world vector
	//	int sx, sy;
	//	world_to_scr(wv, sx, sy);
	//	
	//	//fl_vertex(sx + ssx, sy + ssy);
	//	fl_vertex(sx, sy);
	//}
	//fl_end_line();
}

// NOTE(daniel): [something]_<space2d>_[position]
//                    ^           ^         ^
// --------------------
//                                |         | 
// --------------------------------
//                                          |
// ------------------------------------------
// something: mouse, screen, shape.
// space2d: ex: screen space(scr), world space(world)
// position or coordinate: x, y, w, h, vector, point

class View2D : public Fl_Box {
public:
    View2D(int x, int y, int w, int h, std::vector<Shape*> &p_shapes);

    void world_to_scr(Vector2f world, int &scrx, int &screeny);
    void scr_to_world(int scrx, int screeny, Vector2f& world);
    void get_cursor_v2d_position(int &cx, int &cy);
    static void draw_axes(int centerx, int centery, int w, int h, int line_width);
    Vector2f get_snap_grid(Vector2f vec2d_world);
    void draw_grid(int point_sz);
    void get_mouse();
    void draw() override;

    void clear() { shapes.clear(); redraw(); }
    int handle(int evt) override;
    int handle_draw_mode(int evt);
    void set_cursor();

    void pan(int scrx, int scry);
    void zoom(float scale_factor_percent, int centerx, int centery);

    // NOTE(daniel): The window that we are
    //Fl_Double_Window *mwnd;

    Fl_Surface_Device *this_surface_device;
    Fl_Image_Surface *dev_scr_buf;

    // TODO(daniel): To be implemented minimum widget size
    int min_w = 680;
    int min_h = 360;

    // NOTE(daniel): This widget position are relative to the window
    int v2d_x, v2d_y;
    int v2d_w, v2d_h;

    View2DState state;
    bool changed = false;

    Vector2f axes_center { 0.0f, 0.0f };       // Axes center point

    Vector2f world_offset{ 0.0f, 0.0f };
    float world_scale{ 15.0f };

    // NOTE(daniel): Mouse states, the screen coordinates are relative to this
    // widget
    Pointi mouse_v2d{ 0, 0 };
    Vector2f mouse_world;
    Vector2f mouse_snap_world{ 0.0f, 0.0f };
    Vector2f &mouse_current_world;

    void set_mouse_current_world(Vector2f &mouse);
    Pointi snap_cursor_v2d{ 0, 0 };
    Pointi snap_mouse_scr_pos{ 0, 0 };

    /* Cursor */
    Fl_Cursor current_cursor = FL_CURSOR_DEFAULT;
    Fl_Cursor last_cursor = current_cursor;
    void change_cursor(Fl_Cursor c);

    // TODO(daniel): Make a structure with the mouse states
    bool m_drag_constraint = false;
    Vector2f drag_start_world;

    float drag_sx = 0, drag_sy = 0;// drag start position
    int drag_start_scr_x = 0, drag_start_scr_y = 0; 

    int drag_sxi = 0, drag_syi = 0;// drag start whole screen position
    bool is_dragging = false;
    Mode m_lm_state{ Mode::zoom };

    std::string m_md_scr_msg{ "zoom" };
    float pixel_size = 1.0f;

    // Grid
    // TODO(daniel): Update grid interval based on a threshold
    float grid_interval = pixel_size;
    float snap_grid_interval = grid_interval;
    bool show_grid = true;
    bool is_snap_grid = true;

    BBox *select_shape_bbox = nullptr;
    bool is_selecting = false;
    bool is_moving = false;

    InkbreakerState *app_state;

    bool is_drawing       = false;
    BezierHandle *active_bhandle = nullptr;
    Node *active_point = nullptr;

    Node *active_bpoint = nullptr;             // Active Bezier point
    Node *start_bpoint = nullptr;              // Active Bezier point
    Node *active_head_bhandle = nullptr;       // Head Bezier handle for bezier drawing
    Node *tail_bhandle = nullptr;              // Tail Bezier handle for bezier drawing

    Shape *temp_shape     = nullptr;
    Bezier *bezier_temp_shape     = nullptr;

    // IMPORTANT(daniel): This is a reference to a parent container shapes
    std::vector<Shape*> shapes;
    std::vector<Bezier*> bshapes;
    ShapeInfo shape_info{ 1, FL_BLACK, FL_BLUE, true, true };

    float max_zoom = 300.0f;
    float min_zoom = 0.001f;
    // NOTE(daniel): Scrolling and keyboard zooming in and out factor and sensitivity, 1.0
    // correspont to 100%. Ultra imprecise
    float zooming_factor = 0.10f;  // Scroll zoom
    float zooming_sens   = 0.01f;  // Drag per pixel zoom 
private:
    void DEBUG_display_v2d_info();
    Fl_Offscreen scr_buf;        // Canvas buffer
    // NOTE(daniel): This is a FLTK offscreen implementation detail, I think is
    // to set the DPI factor
    float fl_offscr_scale = 1.0f;
};
