#include <iostream>

#include <stdlib.h>
#include <time.h>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_RGB_Image.H>

#include <FL/platform.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/Enumerations.H>

//
// MAIN WINDOW CALLBACKS DECLARATIONS
//
void new_cb(Fl_Widget* widget, void*);
void save_cb(Fl_Widget* widget, void*);
void saveas_cb(Fl_Widget* widget, void*);
void quit_cb(Fl_Widget* widget, void*);
void about_cb(Fl_Widget* widget, void*);


//
// MAIN WINDOW WIDGETS
//
Fl_Menu_Item menutable[] = {
    { "INKBREAKER", 0, (Fl_Callback*)about_cb, nullptr },
    { "&File",      0, nullptr, nullptr, FL_SUBMENU },
        { "&New",        FL_COMMAND + 'n', (Fl_Callback*)new_cb },
        { "&Save",       FL_COMMAND + 's', (Fl_Callback*)save_cb },
        { "&Save as...", FL_COMMAND + FL_ALT + 's', (Fl_Callback*)saveas_cb },
        { "Quit", FL_COMMAND + 'q', (Fl_Callback*)quit_cb },
    { 0 },
    { "&Help", 0, nullptr, nullptr, FL_SUBMENU },
        { "&About", 0, (Fl_Callback*)about_cb },
        { 0 },
    { 0 }
};

//
// CANVAS
//
Fl_Double_Window* main_window;
constexpr int canvas_size = 200;    // temp
constexpr int num_iterations = 300;
constexpr double max_line_width = 9.0;


class Canvas : public Fl_Box {
public:
    Canvas(int wdx, int wdy, int wdw, int wdh, int cw, int ch);
    void canvas_drawing(void);
    bool has_offscreen() const { return m_offscreen; }
    float scale() const { return m_scale; }
private:
    // member functions
    void draw();
    int handle(int evt);
    double random_val(int v) const;
    // member variables
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

/*
*/
Canvas::Canvas(int wdx, int wdy, int wdw, int wdh, int cw, int ch) :
    Fl_Box(wdx, wdy, wdw, wdh),                 // base box
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
    
    // then add the offscreen on top of the grey background
    if (has_offscreen()) {
        if (m_scale != Fl_Graphics_Driver::default_driver().scale()) {
            // the screen scaling factor has changed
            fl_rescale_offscreen(m_offscreen);
            m_scale = Fl_Graphics_Driver::default_driver().scale();
        }
        fl_copy_offscreen(xo, yo, wd, ht, m_offscreen, m_page_x, m_page_y);
    }
    else {
        // create offscreen
        // some hosts may need a valid window context to base the offscreen on...
        main_window->make_current();
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
            main_window->cursor(FL_CURSOR_MOVE);
            ret = 1;
            break;
        case FL_LEAVE:
            main_window->cursor(FL_CURSOR_DEFAULT);
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
//
// MAIN FUNCTION
//
constexpr int MAIN_WIN_W = 480;
constexpr int MAIN_WIN_H = 360;
constexpr int MENUBAR_W = MAIN_WIN_W;
constexpr int MENUBAR_H = 30;
constexpr int CANVAS_W = 480;
constexpr int CANVAS_H = 360;

static void canvas_animation(void*);
constexpr double delta_time = 0.1;
Canvas* canvas = new Canvas{ 0, MENUBAR_H, MAIN_WIN_W,MAIN_WIN_H, CANVAS_W, CANVAS_H };
// Canvas* canvas = new Canvas{ 0, MENUBAR_H, MAIN_WIN_W, MAIN_WIN_H };
int main(void)
{
    Fl_Menu_Bar menubar{ 0, 0, MENUBAR_W, MENUBAR_H };
    menubar.menu(menutable);
    main_window = new Fl_Double_Window{ MAIN_WIN_W, MAIN_WIN_H };
    main_window->begin();
    main_window->add(menubar);
    main_window->add(canvas);
    main_window->end();
    main_window->resizable(canvas);
    
    main_window->show();
    Fl::add_timeout(delta_time, canvas_animation);
    return Fl::run();
}

static void canvas_animation(void *)
{
    canvas->canvas_drawing();   // if the offscreen exists, draw something
    Fl::repeat_timeout(delta_time, canvas_animation);
}
//
// MAIN WINDOW CALLBACKS DEFINITIONS
//
void new_cb(Fl_Widget* widget, void*) { }
void save_cb(Fl_Widget* widget, void*) { }
void saveas_cb(Fl_Widget* widget, void*) { }
void quit_cb(Fl_Widget* widget, void*)
{
    std::cout << "Bye Bye." << std::endl;
    exit(0);
}
void about_cb(Fl_Widget* widget, void*)
{
    std::cout << "INKBREAKER\n Version 0.0.1\n";
}

class InkBreakerWindow : public Fl_Window {
public:
    InkBreakerWindow(int w = 512, int h = 512)
        : Fl_Window{w, h, "INKBREAKER"}
    {
        m_menu = new Fl_Menu_Bar{0, 0, w, 30};
        m_menu->copy(menutable);
    }
    // ~InkBreakerWindow();
private:
    const char* m_win_name;
    Fl_Menu_Bar* m_menu;
};