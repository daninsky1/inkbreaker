#pragma once

#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <climits>

#include <FL/Fl_Window.H>

#include <FL/Fl_Widget.H>
#include <FL/Fl_Box.H>
// #include <FL/Fl_RGB_Image.H>
#include <FL/platform.H>
#include <FL/fl_draw.H>
#include <FL/Enumerations.H>


// TODO: make this class the world space

#define ZERO_MODE 0
#define DRAG_MODE 1
#define ZOOM_MODE 2
class Canvas : public Fl_Box {
public:
    Canvas(int wdx, int wdy, int wdw, int wdh, int cw, int ch, Fl_Widget* win_wd);
    void canvas_drawing(void);
    bool has_offscreen() const { return m_offscreen; }
    float scale() const { return m_scale; }
    void set_drag_limits(bool v) { m_drag_limits = v; }
private:
    // member functions
    void draw();
    int handle(int evt);
    double random_val(int v) const;
    void canvas_to_screen(int &canvas_w, int &canvas_h);
    void screen_to_canvas(int &canvas_w, int &canvas_h);
    // member variables
    // the window that canvas are
    Fl_Widget* m_win_widget;
    // The offscreen surface
    Fl_Offscreen m_offscreen;
    // variables used to handle "dragging" of the view within the box
    int m_start_x, m_start_y;           // mouse start position on screen space
    bool m_drag_state;                  // non-zero if drag is in progress
    bool m_drag_limits;                 // toggle drag border limits
    int m_canvas_x, m_canvas_y;         // canvas offsets
    int m_canvas_w, m_canvas_h;         // canvas size
    int m_offscreen_w, m_offscreen_h;   // width and height of the offscreen surface buffer
    
    float m_scale_x, m_scale_y;
    int m_zoom_xoff;                        // zoom mouse x offset
    bool m_zoom_state;     // non-zero if zoom is in progress
    float m_scale;    // current screen scaling factor value
    // TODO: undock canvas drawing function
    int m_canvas_mode;
};