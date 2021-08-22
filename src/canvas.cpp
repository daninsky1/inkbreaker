#include "canvas.h"

#define STB_PERLIN_IMPLEMENTATION
#include "stb/stb_perlin.h"


constexpr int canvas_size = 200;    // temp

constexpr double max_line_width = 9.0;

Canvas::Canvas(int wdx, int wdy, int wdw, int wdh, int cw, int ch, Fl_Widget* win_wd) :
    Fl_Box{wdx, wdy, wdw, wdh},     // base box
    m_win_widget{win_wd},
    m_offscreen{0},
    m_start_x{0}, m_start_y{0},
    m_drag_state{false},
    m_drag_limits{false},
    m_canvas_x{-wdw/2}, m_canvas_y{-wdh/2},
    m_canvas_w{cw}, m_canvas_h{ch},
    m_offscreen_w{0}, m_offscreen_h{0},
    m_zoom_state{false},
    m_scale{1.0f},
    m_canvas_mode{DRAG_MODE}
{ } // Canvas

void Canvas::draw()
{
    int wd = w();   // gets the widget width
    int ht = h();   // gets the widget height
    int xo = x();   // gets the widget x position relative to the window
    int yo = y();   // gets the widget y position relative to the window
    
    // draw rectangle, the actual canvas
    fl_color(FL_DARK_RED);
    fl_rectf(xo, yo, wd, ht);        // fill screen space background

    // then add the canvas on top of the background
    if (has_offscreen()) {
        if (m_scale != Fl_Graphics_Driver::default_driver().scale()) {
            // the screen scaling factor has changed
            Fl_Graphics_Driver::default_driver().scale(m_scale);
            fl_rescale_offscreen(m_offscreen);
            float default_sc_factor = 1.0f;
            Fl_Graphics_Driver::default_driver().scale(default_sc_factor);
        }
        fl_copy_offscreen(xo, yo, wd, ht, m_offscreen, m_canvas_x, m_canvas_y);
    }
    else {
        // create offscreen buffer "world space"
        // some hosts may need a valid window context to base the offscreen on...
        std::cout << "New Offscreen\n";
        static_cast<Fl_Window*>(m_win_widget)->make_current();
        m_offscreen_w = m_canvas_w;
        m_offscreen_h = m_canvas_h;
        m_offscreen = fl_create_offscreen(m_offscreen_w, m_offscreen_h);
        // m_scale = Fl_Graphics_Driver::default_driver().scale();
    }
    // LOG
    fl_color(FL_WHITE);
    int font = FL_COURIER;
    int font_sz = 14;
    fl_font(font, 14);
    char c_draw_log_msg[100];
    sprintf(c_draw_log_msg,
            "Screen Position: (%d, %d) | Screen Size: (%d, %d)",
            xo, yo, wd, ht);
    fl_draw(c_draw_log_msg, xo, yo+fl_height(font, font_sz));
    sprintf(c_draw_log_msg,
            "Canvas Position: (%d, %d) | Canvas Size: (%d, %d)",
            m_canvas_x, m_canvas_y, m_canvas_w, m_canvas_h);
    fl_draw(c_draw_log_msg, xo, yo+fl_height(font, font_sz)*2);
    sprintf(c_draw_log_msg,
            "Canvas Offsets: (%d, %d)",
            m_canvas_x, m_canvas_y);
    fl_draw(c_draw_log_msg, xo, yo+fl_height(font, font_sz)*3);
} // draw member

int Canvas::handle(int evt)
{
    /* Calling the inherited handle() method in overridden method so that doesn't
    short-circuit events that this doesn't handle.
    In this last case we return callee retval.
    */
    int ret = Fl_Box::handle(evt);

    // focus keyboard events to this widget
    switch (evt) {
        case FL_FOCUS:
            return 1;
            break;
        case FL_UNFOCUS:
            break;
        default:
            break;     
    }
    
    // test and print keys
    int key_code = 0;
    if (evt == FL_KEYBOARD) {
        char buffer[100];
        const char *keyname = buffer;
        key_code = Fl::event_key();
        // ONLY HANDLES ASCII EVENT KEYS !!!!!!!
        if (key_code < 128) { // ASCII
            sprintf(buffer, "'%c'", key_code);
            switch (key_code) {
            case 'z':
                std::cout << "ZOOM_MODE\n";
                m_canvas_mode = ZOOM_MODE;
                break;
            case 'h': case ' ':
                std::cout << "DRAG_MODE\n";
                m_canvas_mode = DRAG_MODE;
                break;
            case '0':
                std::cout << "ZERO_MODE\n";
                m_canvas_mode = ZERO_MODE;
                break;
            }
        } else if (evt >= 0xa0 && evt <= 0xff) { // ISO-8859-1 (international keyboards)
            char key_buf[8];
            int kl = fl_utf8encode((unsigned)evt, key_buf);
            key_buf[kl] = '\0';
            sprintf(buffer, "'%s'", key_code);
            ret = 1;
        } else if (key_code > FL_F && key_code <= FL_F_Last) {
            sprintf(buffer, "FL_F+%d", key_code - FL_F);
            ret = 1;
        } else if (key_code >= FL_KP && key_code <= FL_KP_Last) {
            sprintf(buffer, "FL_KP+'%c'", key_code-FL_KP);
            ret = 1;
        } else if (key_code >= FL_Button && key_code <= FL_Button+7) {
            sprintf(buffer, "FL_Button+%d", key_code-FL_Button);
            ret = 1;
        }
        std::cout << keyname << '\t' << key_code << '\n';
    }
    
    
    
    switch (m_canvas_mode) {
    case DRAG_MODE:
        if (has_offscreen()) {
            switch (evt) {
            case FL_ENTER:
                static_cast<Fl_Window*>(m_win_widget)->cursor(FL_CURSOR_MOVE);
                ret = 1;
                break;
            case FL_LEAVE:
                static_cast<Fl_Window*>(m_win_widget)->cursor(FL_CURSOR_DEFAULT);
                ret = 1;
                break;
            case FL_PUSH:
                m_start_x = Fl::event_x_root();
                m_start_y = Fl::event_y_root();
                m_drag_state = true;
                ret = 1;
                break;
            case FL_DRAG:
                if (m_drag_state) {
                    int update_mouse_x = Fl::event_x_root();
                    int update_mouse_y = Fl::event_y_root();
                    // drag difference
                    m_canvas_x -= (update_mouse_x - m_start_x);
                    m_canvas_y -= (update_mouse_y - m_start_y);
                    m_start_x = update_mouse_x;
                    m_start_y = update_mouse_y;
                    // log
                    // std::cout << "screen_space_pos: x" << m_start_x << "|y: " << m_start_y << " | "
                    //           << "mouse event_root: x" << update_mouse_x << "|y: " << update_mouse_y << " | "
                    //           << "drag offsets: " << m_start_x << "|y: " << m_start_y << '\n';
                    // check the page bounds
                    if (m_drag_limits) {
                        if (m_canvas_x < -w()) m_canvas_x = -w();
                        else if (m_canvas_x > m_offscreen_w) m_canvas_x = m_offscreen_w;
                        if (m_canvas_y < -h()) m_canvas_y = -h();
                        else if (m_canvas_y > m_offscreen_h) m_canvas_y = m_offscreen_h;
                    }
                    redraw();
                }
                ret = 1;
                break;
            case FL_RELEASE:
                m_drag_state = false;
                ret = 1;
                break;
            default:
                break;
            }
        }
        break;
    case ZOOM_MODE:
        if (has_offscreen()) {
            switch (evt) {
            case FL_ENTER:
                static_cast<Fl_Window*>(m_win_widget)->cursor(FL_CURSOR_MOVE);
                ret = 1;
                break;
            case FL_LEAVE:
                static_cast<Fl_Window*>(m_win_widget)->cursor(FL_CURSOR_DEFAULT);
                ret = 1;
                break;
            case FL_Button+4:   // scrool up
                m_start_x = Fl::event_x_root();
                m_zoom_state = true;
                ret = 1;
                break;
            case FL_Button+5:   // scrool down
                if (m_zoom_state) {
                    int m_zoom_x2 = Fl::event_x_root();
                    
                    m_zoom_xoff = m_start_x - m_zoom_x2;
                    m_start_x = m_zoom_x2;
                    // check the page bounds
                    m_scale = m_zoom_x2;
                    if (m_canvas_x < -w()) m_scale = -w();
                    else if (m_canvas_x > m_offscreen_w) m_canvas_x = m_offscreen_w;
                    redraw();
                }
                ret = 1;
                break;
            case FL_RELEASE:
                m_zoom_state = false;
                ret = 1;
                break;
            default:
                break;
            }
        }
        break;
    case ZERO_MODE:
        break;
    default:
        break;
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



float zoff = 0.0f;

void Canvas::canvas_drawing(void)
{
    if (!has_offscreen()) return;   // no valid offscreen, nothing to do here

    fl_begin_offscreen(m_offscreen);    // open the offscreen context for drawing
    
    fl_color(FL_GRAY);
    fl_rectf(0, 0, 480, 360);
    fl_color(FL_GREEN);
    fl_pie(50, 50, 50, 50, 0, 360);
    
    /*  // PERLIN NOISE
    // create image buffer on the stack
    int depth = 3;
    const size_t ibuffer_sz = m_canvas_w*m_canvas_h*depth;
    const size_t img_row_size = m_canvas_w*depth;
    unsigned char ibuffer[ibuffer_sz];
    // fill image
    for (size_t i = 0; i < ibuffer_sz; i+=depth) {
        ibuffer[i] = 255;    // R
        ibuffer[i+1] = 0;    // G
        ibuffer[i+2] = 0;    // B
    }

    float detail = 0.025f;
    float movement_pace = 0.1;
    int seed = 43;
    
    auto normalize_range = [](float value) {
        value++;
        float perlin_min = 0.0f;
        float perlin_max = 2.0f;
        unsigned char result = static_cast<unsigned char>(((value / perlin_max) * static_cast<float>(CHAR_MAX)));
        return result;
    };
    float yoff = 1.0f;
    
    for (size_t i = 0; i < m_canvas_h; ++i) {
        float xoff = 0.0f;
        for (size_t j = 0; j < img_row_size; j+=depth) {
            auto v = normalize_range(stb_perlin_noise3_seed(xoff, yoff, zoff, 0, 0, 0, seed));
            xoff+=detail;
            // std::cout << +v << '\n';
            size_t ln_i = i*img_row_size+j;   // linear index (1d index)
            ibuffer[ln_i] = v;   // R
            ibuffer[ln_i+1] = v;    // G
            ibuffer[ln_i+2] = v;    // B
        }
        yoff+=detail;
    }
    zoff+=movement_pace;

    fl_draw_image(ibuffer, 0, 0, m_canvas_w, m_canvas_h, depth);
    */
   
   
    /*  SPLASH SCREEN
    constexpr int num_iterations = 300;
    constexpr int first_useful_color = 56;
    constexpr int last_useful_color = 255;
    Fl_Color color;
    static int icolor = first_useful_color;
    static int ox = canvas_size / 2;    // begin in the middle of the screen
    static int oy = canvas_size / 2;
    
    
    int iters = 
    
    if (m_iters > num_iterations) { // clear the offscreen and start afresh
        fl_color(FL_WHITE);
        fl_rectf(0, 0, m_offscreen_w, m_offscreen_h);
        m_iters = 0;
    }
    m_iters++;
        set the color
    icolor++;
    if (icolor > last_useful_color) icolor = first_useful_color;
    color = static_cast<Fl_Color>(icolor);
    fl_color(color);

    double drx = random_val(m_offscreen_w);
    double dry = random_val(m_offscreen_h);
    double drt = random_val(max_line_width);

    int ex = static_cast<int>(drx);
    int ey = static_cast<int>(dry);
    fl_line_style(FL_SOLID, static_cast<int>(drt));
    fl_line(ox, oy, ex, ey);
    ox = ex;
    oy = ey;
    
    */
    fl_line_style(FL_SOLID, 0);
    fl_end_offscreen();     // close the offscreen context
    redraw();
} // canvas_drawing

// TODO: implement conversions
void Canvas::canvas_to_screen(int &canvas_w, int &canvas_h)
{
    
}
void Canvas::screen_to_canvas(int &canvas_w, int &canvas_h)
{
    
}