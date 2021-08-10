#pragma once

#include <stdlib.h>
#include <iostream>

#include <FL/Fl_Window.H>

#include <FL/Fl_Box.H>
#include <FL/platform.H>
#include <FL/fl_draw.H>

class Canvas : public Fl_Box {
public:
    Canvas(int wdx, int wdy, int wdw, int wdh, int cw, int ch, Fl_Widget* win_wd);
    void canvas_drawing(void);
    bool has_offscreen() const { return m_offscreen; }
    float scale() const { return m_scale; }
private:
    // member functions
    void draw();
    int handle(int evt);
    double random_val(int v) const;
    // member variables
    // the window that canvas are
    Fl_Widget* m_win_widget;
    // The offscreen surface
    Fl_Offscreen m_offscreen;
    // variables used to handle "dragging" of the view within the box
    int m_x1, m_y1;     // drag start positions
    int m_xoff, m_yoff; // drag offsets
    int m_drag_state; // non-zero if drag is in progress
    int m_page_x, m_page_y; // top left of view area
    int m_canvas_w, m_canvas_h;
    int m_offscreen_w, m_offscreen_h;   // width and height of the offscreen surface
    int m_iters;      // must be set on first pass!
    float m_scale;    // current screen scaling factor value
};