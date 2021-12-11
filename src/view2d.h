#pragma once

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
    circle
};

enum class Select{
    move,
    scale,
    rotate
};

struct View2DState {
    Mode mode;
    Draw draw;
    Select select;
};
//
// THE SCREEN CONTEXT
//
class View2D : public Fl_Box {
public:
    View2D(int x, int y, int w, int h, std::vector<Shape*> &p_shapes);


    void world_to_scr(Vector world, int &scrx, int &screeny);
    void scr_to_world(int scrx, int screeny, Vector& world);

    void draw();
    void draw_create_shape();

    void clear() { shapes.clear(); redraw(); }

    int handle(int evt);
    void set_cursor();

    void pan();
    void zoom(int focusx, int focusy, float scale_factor_percent);

    // NOTE(daniel): The window that we are
    //Fl_Double_Window *mwnd;

    Fl_Surface_Device *this_surface_device;
    Fl_Image_Surface *dev_scr_buf;

    // TODO(daniel): To be implemented minimum widget size
    int min_w = 680;
    int min_h = 360;

    // NOTE(daniel): This widget position are relative to the window
    int ssx, ssy;
    int ssw, ssh;

    View2DState state{ Mode::draw, Draw::rect };
    bool changed = false;

    // NOTE(daniel): World in relation to the screen
    Vector world_offset{ 0.0, 0.0 };
    float world_scale{ 1.0f };

    // NOTE(daniel): Mouse states, the screen coordinates are relative to this
    // widget
    Pointi m_mouse_scr_pos{ 0, 0 };
    Vector m_mouse_world_pos;
    Pointi snap_mouse_scr_pos{ 0, 0 };
    Vector snap_mouse_world_pos;

    /* Cursor */
    Fl_Cursor current_cursor = FL_CURSOR_DEFAULT;
    Fl_Cursor last_cursor = current_cursor;
    void change_cursor(Fl_Cursor c);

    // TODO(daniel): Make a structure with the mouse states
    bool m_drag_constraint = false;
    Vector drag_world_start_pos;
    float drag_sx = 0, drag_sy = 0;// drag start position
    bool is_dragging = false;
    Mode m_lm_state{ Mode::zoom };

    std::string m_md_scr_msg{ "zoom" };
    float pixel_size = 1.0f;
    float grid_snap_interval = pixel_size;
    float visual_grid_interval;

    SelectBox *select_box = nullptr;
    bool is_selecting = false;
    bool is_moving = false;

    InkbreakerState *app_state;

    bool is_drawing       = false;
    Node *m_selected_node = nullptr;
    Shape *temp_shape     = nullptr;

    // IMPORTANT(daniel): This is a reference to a parent container shapes
    std::vector<Shape*> &shapes;
    ShapeInfo sinfo{ 1, FL_BLACK, FL_BLUE };

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
