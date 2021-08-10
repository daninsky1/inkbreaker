#include "canvas.h"

constexpr int canvas_size = 200;    // temp
constexpr int num_iterations = 300;
constexpr double max_line_width = 9.0;

/*
*/
Canvas::Canvas(int wdx, int wdy, int wdw, int wdh, int cw, int ch, Fl_Widget* win_wd) :
    Fl_Box(wdx, wdy, wdw, wdh),                 // base box
    m_win_widget{win_wd},
    m_offscreen{0},                     // offscreen is not set at start
    m_x1{0}, m_y1{0}, m_drag_state{0},  // not dragging view
    m_page_x{0},                        // roughly centred in view
    m_page_y{0},
    m_canvas_w{cw}, m_canvas_h{ch},
    m_offscreen_w{0}, m_offscreen_h{0}, // offscreen size
    m_iters{num_iterations + 1}
{ 
    
} // Canvas

void Canvas::draw()
{
    int wd = w();   // gets the widget width
    int ht = h();   // gets the widget height
    int xo = x();   // gets the widget x position relative to the window
    int yo = y();   // gets the widget y position relative to the window
    
    // draw world color
    fl_color(fl_gray_ramp(255));     // a black background shade
    // draw rectangle, the actual canvas
    fl_rectf(xo, yo, wd, ht);        // fill the box with this colour
    
    // then add the canvas on top of the grey background
    if (has_offscreen()) {
        if (m_scale != Fl_Graphics_Driver::default_driver().scale()) {
            // the screen scaling factor has changed
            fl_rescale_offscreen(m_offscreen);
            m_scale = Fl_Graphics_Driver::default_driver().scale();
        }
        fl_copy_offscreen(xo, yo, wd, ht, m_offscreen, m_page_x, m_page_y);
    }
    else {
        // create offscreen buffer
        // some hosts may need a valid window context to base the offscreen on...
        std::cout << "New Offscreen\n";
        static_cast<Fl_Window*>(m_win_widget)->make_current();
        m_offscreen_w = m_canvas_w;
        m_offscreen_h = m_canvas_h;
        m_offscreen = fl_create_offscreen(m_offscreen_w, m_offscreen_h);
        m_scale = Fl_Graphics_Driver::default_driver().scale();
    }
} // draw member

int Canvas::handle(int evt)
{
    int ret = Fl_Box::handle(evt);
    
    // handle dragging of visible page area - if a valid context exists
    if (has_offscreen()) {
        switch (evt)
        {
        case FL_ENTER:
            static_cast<Fl_Window*>(m_win_widget)->cursor(FL_CURSOR_MOVE);
            ret = 1;
            break;
        case FL_LEAVE:
            static_cast<Fl_Window*>(m_win_widget)->cursor(FL_CURSOR_DEFAULT);
            ret = 1;
            break;
        case FL_PUSH:
            m_x1 = Fl::event_x_root();
            m_y1 = Fl::event_y_root();
            m_drag_state = true;
            ret = 1;
            break;
        case FL_DRAG:
            if (m_drag_state) {
                int m_x2 = Fl::event_x_root();
                int m_y2 = Fl::event_y_root();
                m_xoff = m_x1 - m_x2;
                m_yoff = m_y1 - m_y2;
                m_x1 = m_x2;
                m_y1 = m_y2;
                m_page_x += m_xoff;
                m_page_y += m_yoff;
                // check the page bounds
                if (m_page_x < -w()) m_page_x = -w();
                else if (m_page_x > m_offscreen_w) m_page_x = m_offscreen_w;
                if (m_page_y < -h()) m_page_y = -h();
                else if (m_page_y > m_offscreen_h) m_page_y = m_offscreen_h;
                redraw();
            }
            ret = 1;
            break;
        case FL_RELEASE:
            m_drag_state = 0;
            ret = 1;
            break;
        default:
            break;
        }
    }
    return ret;
} // handle member

double Canvas::random_val(int v) const
{
    // 0.0 to 1.0
    double dr = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
    dr = dr * static_cast<double>(v);   // 0 to v
    return dr;
}


constexpr int first_useful_color = 56;
constexpr int last_useful_color = 255;

void Canvas::canvas_drawing(void)
{
    Fl_Color color;
    static int icolor = first_useful_color;
    static int ox = canvas_size / 2;
    static int oy = canvas_size / 2;
    if (!has_offscreen()) return;   // no valid offscreen, nothing to do here
    
    fl_begin_offscreen(m_offscreen);    // open the offscreen context for drawing
    {
        if (m_iters > num_iterations) { // clear the offscreen and start afresh
            fl_color(FL_WHITE);
            fl_rectf(0, 0, m_offscreen_w, m_offscreen_h);
            m_iters = 0;
        }
        m_iters++;
        
        icolor++;
        if (icolor > last_useful_color) icolor = first_useful_color;
        color = static_cast<Fl_Color>(icolor);
        fl_color(color);    // set the color
        
        double drx = random_val(m_offscreen_w);
        double dry = random_val(m_offscreen_h);
        double drt = random_val(max_line_width);
        
        int ex = static_cast<int>(drx);
        int ey = static_cast<int>(dry);
        fl_line_style(FL_SOLID, static_cast<int>(drt));
        fl_line(ox, oy, ex, ey);
        ox = ex;
        oy = ey;
    }
    fl_line_style(FL_SOLID, 0);
    fl_end_offscreen();     // close the offscreen context
    redraw();
} // canvas_drawing