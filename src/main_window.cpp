#define INKBREAKER_VERSION_MAJOR 0
#define INKBREAKER_VERSION_MINOR 0
#define INKBREAKER_VERSION_PATCH 1

#include <math.h>
#include "main_window.h"



void save_cb(Fl_Widget* widget, void*)
{
}

void controls_cd(Fl_Widget* widget, void*)
{
    fl_message("change left mouse button mode:\n"
        "pan: h or space or middle mouse button directly\n"
        "zoom: z\n"
        "nothing: 0");
}

void about_cb(Fl_Widget* widget, void*)
{
    std::cout << "INKBREAKER - "
        << "Version " << INKBREAKER_VERSION_MAJOR << "."
        << INKBREAKER_VERSION_MINOR << "."
        << INKBREAKER_VERSION_PATCH << std::endl;
}

void clear_cb(Fl_Widget* widget, void* mwv)
{
    MainWindow* mwnd = static_cast<MainWindow*>(mwv);
}
void pan_state_cb(Fl_Widget* widget, void*);
void zoom_state_cb(Fl_Widget* widget, void*);

void line_state_cb(Fl_Widget* widget, void* mwv)
{
    MainWindow* mwnd = static_cast<MainWindow*>(mwv);
    mwnd->v2d->state.mode = Mode::draw;
    mwnd->v2d->state.draw = Draw::line;
}
void rect_state_cb(Fl_Widget* widget, void* mwv)
{
    MainWindow* mwnd = static_cast<MainWindow*>(mwv);
    mwnd->v2d->state.mode = Mode::draw;
    mwnd->v2d->state.draw = Draw::rect;
}
void circle_state_cb(Fl_Widget* widget, void* mwv)
{
    MainWindow* mwnd = static_cast<MainWindow*>(mwv);
    mwnd->v2d->state.mode = Mode::draw;
    mwnd->v2d->state.draw = Draw::circle;
}

void set_line_color_cb(Fl_Widget* widget, void* mwv)
{
    MainWindow* mwnd = static_cast<MainWindow*>(mwv);
    Fl_Color oldcl = mwnd->v2d->sinfo.line_color;
    Fl_Color c = fl_show_colormap(FL_BLACK);
    mwnd->v2d->sinfo.line_color = c;
}
void set_fill_color_cb(Fl_Widget* widget, void* mwv)
{
    MainWindow* mwnd = static_cast<MainWindow*>(mwv);
    Fl_Color oldcl = mwnd->v2d->sinfo.fill_color;
    Fl_Color c = fl_show_colormap(FL_BLACK);
    mwnd->v2d->sinfo.fill_color = c;
}


//void draw_msg(char *msg, float wsxf, float wsyf, int fnt_sz, float scale)
//{
//	fl_font(FL_TIMES_BOLD_ITALIC, static_cast<Fl_Fontsize>(fnt_sz*scale));
//
//    // world space text float
//    // screen space text int
//	int ssxi, ssyi;
//	world_to_scr(wsxf, wsxf, ssxi, ssyi);
//	fl_draw(msg, ssxi, ssyi);
//}

Fl_Menu_Item menu_items[] = {
    { "INKBREAKER", 0, (Fl_Callback*)about_cb, nullptr, FL_MENU_INACTIVE },
    { "&File",      0, nullptr, nullptr, FL_SUBMENU },
        { "&New",        FL_COMMAND + 'n', (Fl_Callback*)new_cb },
        { "&Save",       FL_COMMAND + 's', (Fl_Callback*)save_cb },
        { "&Save as...", FL_COMMAND + FL_ALT + 's', (Fl_Callback*)saveas_cb },
        { "Quit", FL_COMMAND + 'q', (Fl_Callback*)quit_cb },
        { 0 },
    // TODO: Implement procedural texture generation after deployment to image support
    // called InkBlend
    /*
    { "InkBlend", 0, nullptr, nullptr, FL_SUBMENU },
        { "Blank", 0, (Fl_Callback*)blank_cb},
        { "Line Splash Animation", 0, (Fl_Callback*)perlin_noise_anim_cb },
        { "Perlin Noise Animation", 0, (Fl_Callback*)perlin_noise_anim_cb },
        { 0 },
    */
    { "Mode", 0, nullptr, nullptr, FL_SUBMENU },
        { "Pan", 0, (Fl_Callback*)pan_state_cb},
        { "Zoom", 0, (Fl_Callback*)zoom_state_cb},
        { 0 },
    { "View", 0, nullptr, nullptr, FL_SUBMENU },
        { "Grid", 0, (Fl_Callback*)pan_state_cb},
        { 0 },
    { "Draw", 0, nullptr, nullptr, FL_SUBMENU },
        { "Line", 0, (Fl_Callback*)line_state_cb},
        { "Rectangle", 0, (Fl_Callback*)rect_state_cb},
        { "Circle", 0, (Fl_Callback*)circle_state_cb},
        { 0 },
    { "Shape Style", 0, nullptr, nullptr, FL_SUBMENU },
    //    { "Line Width", 0, nullptr, nullptr, FL_SUBMENU },
    //        { "1", 0, (Fl_Callback*)set_line_width_cb },
    //        { "2", 0, (Fl_Callback*)set_line_width_cb },
    //        { "3", 0, (Fl_Callback*)set_line_width_cb },
    //        { "4", 0, (Fl_Callback*)set_line_width_cb },
    //        { "5", 0, (Fl_Callback*)set_line_width_cb },
    //        { 0 },
        { "Line Color", 0, (Fl_Callback*)set_line_color_cb },
        { "Fill Color", 0, (Fl_Callback*)set_fill_color_cb },
        { 0 },
    { "&Help", 0, nullptr, nullptr, FL_SUBMENU },
        { "Controls", 0, (Fl_Callback*)controls_cd },
        { "&About", 0, (Fl_Callback*)about_cb },
        { 0 },
    { 0 }
};



MainWindow::MainWindow() :
    Fl_Double_Window{ V2D_DEFAULT_W, MENU_BAR_H + V2D_DEFAULT_H, "MainWindow" }
{
    // IMPORTANT(daniel): Passing this pointer to v2d is really shady. For
    // reasons that I not fully understand. Constructing the v2d after menu bar
    // causes read violation
    v2d = new View2D{0, MENU_BAR_H, V2D_DEFAULT_W, V2D_DEFAULT_H + MENU_BAR_H, this};

    for (int i = 0; i < menu_items->size(); ++i) {
        if (menu_items[i].label()) {
            menu_items[i].user_data(this);
        }
    }
    menu_bar = new Fl_Menu_Bar{0, 0, V2D_DEFAULT_W, MENU_BAR_H};
    menu_bar->menu(menu_items);
    resizable(v2d);
    menu_bar->redraw();
}


MainWindow::MainWindow(int w, int h, const char* l) :
    Fl_Double_Window{ w, MENU_BAR_H + h, l }
{
    v2d = new View2D{0, MENU_BAR_H, V2D_DEFAULT_W, V2D_DEFAULT_H + MENU_BAR_H, this};
    
    menu_bar = new Fl_Menu_Bar{0, 0, V2D_DEFAULT_W, MENU_BAR_H};
    menu_bar->menu(menu_items);
    resizable(v2d);
    menu_bar->redraw();

    // IMPORTANT(daniel): This can problematic, menu_bar doesn't allow to change
    // the state of its menu_items, so const_cast was used, until I find a better
    // way to pass this object to the menu items callback data
    for (int i = 0; i < menu_bar->size(); ++i) {
        if (menu_bar->menu()[i].label()) {
            Fl_Menu_Item *item = const_cast<Fl_Menu_Item*>(&menu_bar->menu()[i]);
            item->user_data(this);
        }
    }
}

void pan_state_cb(Fl_Widget* widget, void* mwv)
{
    MainWindow *mwnd = static_cast<MainWindow*>(mwv);
    mwnd->v2d->state.mode = Mode::pan;
}
void zoom_state_cb(Fl_Widget* widget, void* mwv)
{
    MainWindow *mwnd = static_cast<MainWindow*>(mwv);
    mwnd->v2d->state.mode = Mode::zoom;
}


