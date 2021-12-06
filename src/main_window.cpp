#define INKBREAKER_VERSION_MAJOR 0
#define INKBREAKER_VERSION_MINOR 0
#define INKBREAKER_VERSION_PATCH 1

#include <math.h>
#include "main_window.h"


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



View2D::View2D(int x, int y, int w, int h, Fl_Double_Window* wnd) :
	Fl_Box{ x, y, w, h },
	m_wnd{ wnd }
{
    scr_buf = fl_create_offscreen(w, h);
    fl_offscr_scale = Fl_Graphics_Driver::default_driver().scale();
    ssx = x; ssy = y;
    ssw = w; ssh = h;
} // View2D

void View2D::draw()
{
    // NOTE(daniel): The drawing functions only works in a limited way, you can
    // only draw on the window coordinates, any other way to control some buffer
    // outside this coordinates in painful or show some artefacts and will not
    // be implement for now, that said the only way is to pass the View2D
    // widget offsets to every fltk draw function calls.
    
    if (ssw != w() || ssh != h()) {
        ssx = x();
        ssy = y();
        ssw = w();
        ssh = h();
        fl_delete_offscreen(scr_buf);
        scr_buf = fl_create_offscreen(ssw, ssh);
    }

    if (fl_offscr_scale != Fl_Graphics_Driver::default_driver().scale()) {
        // the screen scaling factor has changed
        fl_rescale_offscreen(scr_buf);
        fl_offscr_scale = Fl_Graphics_Driver::default_driver().scale();
    }
    
	sShape::world_offset = world_offset;
	sShape::world_scale = world_scale;

    fl_begin_offscreen(scr_buf);
	// DRAW WORLD BACKGROUND
	fl_color(FL_DARK3);
	//fl_rectf(ssx, ssy, ssw + ssx, ssh + ssy);
	fl_rectf(0, 0, ssw, ssh);
	// DRAW GRID
	// TODO: IMPLEMENT GRID

	// DRAW AXES
	Vector ax{ 0.0, 0.0 };
	int axx, axy;
	world_to_scr(ax, axx, axy);
	fl_line_style(FL_SOLID, 1);
	fl_color(FL_RED);
	fl_line(axx, axy, ssw, axy);
	fl_color(FL_GREEN);
	fl_line(axx, axy, axx, ssh);
	fl_line_style(FL_DOT, 1);
	fl_color(FL_RED);
	fl_line(0, axy, axx, axy);
	fl_color(FL_GREEN);
	fl_line(axx, 0, axx, axy);


    // NOTE(daniel): Render queue
    for (int i = 0; i < shapes.size(); ++i) {
        shapes[i]->draw_shape();
    }

	if (temp_shape) {
		temp_shape->draw_shape();
		//temp_shape->draw_nodes();
	}

    // NOTE(daniel): Sine wave
    // TODO(daniel): Wrap this to a function
	fl_color(FL_WHITE);
	fl_line_style(FL_SOLID, 2*(int)world_scale);
	fl_begin_line();
	for (float i = 0; i < w(); i += 0.1f) {
		Vector wv{ i, std::sin(i * (0.5f * 0.1f)) * 50 };	// world vector
		int sx, sy;
		world_to_scr(wv, sx, sy);
		
		//fl_vertex(sx + ssx, sy + ssy);
		fl_vertex(sx, sy);
	}
	fl_end_line();


	// // GRID
	// // TODO: IMPLEMENT GRID OBJECT
	// /*
	// * 
	// fl_line_style(FL_SOLID, 2.0f*world_scale);
	// // horizontal lines
	// float gridx = 100.0f, gridy = 100.0f;
	// for (double y = 0.0; y <= gridy; y+=gridy/10.0) {
	// 	Vector start{ 0.0, y };
	// 	Vector end{ gridx, y };

	// 	// world to screen
	// 	int start_sspx, start_sspy, end_sspx, end_sspy;
	// 	world_to_scr(start, start_sspx, start_sspy);
	// 	world_to_scr(end, end_sspx, end_sspy);

	// 	fl_line(start_sspx, start_sspy, end_sspx, end_sspy);
	// }

	// //// vertical lines
	// for (double x = 0.0; x <= gridx; x+=gridx/10.0) {
	// 	Vector start{ x, 0.0 };
	// 	Vector end{ x, gridy };
	// 	int startx = x, starty = 0.0f;
	// 	int endx = x, endy = gridy;

	// 	// world to screen
	// 	int start_sspx, start_sspy, end_sspx, end_sspy;
	// 	world_to_scr(start, start_sspx, start_sspy);
	// 	world_to_scr(end, end_sspx, end_sspy);

	// 	fl_line(start_sspx, start_sspy, end_sspx, end_sspy);
	// }
	// */
    
    // DRAW SNAP CURSOR
    fl_color(FL_WHITE);
    int snap_x;
    int snap_y;
    world_to_scr(snap_mouse_world_pos, snap_x, snap_y);
    fl_line_style(FL_SOLID, 1);

    if (world_scale >= 15.0f) {
        fl_circle(snap_x, snap_y, 5);
    }

    fl_end_offscreen();
    fl_copy_offscreen(ssx, ssy, ssw, ssh, scr_buf, 0, 0);


	// SCREEN DEBUG INFO
	fl_color(FL_WHITE);
	int font = FL_COURIER;
	int font_sz = 14;
	fl_font(font, 15);

	int pad = 10;
	// screen mouse position
	std::stringstream ss_log;
	ss_log << std::fixed;
	ss_log << "View2D Size: " << '(' << ssw << " x " << ssh << ')';
	fl_draw(ss_log.str().c_str(), ssx + pad, ssy + fl_height(font, font_sz));
	ss_log.str(std::string{""});
	ss_log.str("");
	ss_log << "World Offset: " << '(' << world_offset.x << " ," << world_offset.y << ')';
	fl_draw(ss_log.str().c_str(), ssx + pad, ssy + fl_height(font, font_sz) * 2);
	ss_log.str("");
	ss_log << "Scale: " << world_scale;
	fl_draw(ss_log.str().c_str(), ssx + pad, ssy + fl_height(font, font_sz) * 3);

	// mode
	fl_draw(m_md_scr_msg.c_str(), ssx+pad, h() - pad - fl_height(font, font_sz)*4);
	std::string mouse_coor;
	mouse_coor.append("mouse screen (");
	mouse_coor.append(std::to_string(m_mouse_scr_pos.x));
	mouse_coor.append(" ,");
	mouse_coor.append(std::to_string(m_mouse_scr_pos.y));
	mouse_coor.append(")");
	fl_draw(mouse_coor.c_str(), ssx + pad, h() - pad - fl_height(font, font_sz)*3);
	// world mouse position
	mouse_coor = "";
	mouse_coor.append("mouse world (");
	mouse_coor.append(std::to_string(m_mouse_world_pos.x));
	mouse_coor.append(" ,");
	mouse_coor.append(std::to_string(m_mouse_world_pos.y));
	mouse_coor.append(" )");
	fl_draw(mouse_coor.c_str(), ssx + pad, h() - pad - fl_height(font, font_sz)*2);
	ss_log.str("");
	ss_log << "mouse world snap: (" << snap_mouse_world_pos.x << ", " << snap_mouse_world_pos.y << ')';
	fl_draw(ss_log.str().c_str(), ssx + pad, h() - pad - fl_height(font, font_sz));
	ss_log.str("");
	ss_log << "mouse screen snap: (" << snap_x << ", " << snap_y << ')';
	fl_draw(ss_log.str().c_str(), ssx + pad, h() - pad);
}


int View2D::handle(int evt)
{
    // TODO(daniel): Implement system msg intercept with Fl::add_system_handler()

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
        constexpr int bfsz = 100;
		char buffer[bfsz];
		const char* keyname = buffer;
		key_code = Fl::event_key();
		// ONLY HANDLES ASCII EVENT KEYS !!!!!!!
		if (key_code < 128) { // ASCII
			sprintf_s(buffer, bfsz,"'%c'", key_code);
			switch (key_code) {
			case 'r': {
				std::cout << ssw << ", " << ssh << " " << w() << ", " << h() << std::endl;
				redraw();
			} break;
			case 'z':
				std::cout << "ZOOM_MODE\n";
				m_lm_state = Mode::zoom;
				m_md_scr_msg = "zoom";
				redraw();
				break;
			case 'h': case ' ':
				std::cout << "DRAG_MODE\n";
				m_lm_state = Mode::pan;
				m_md_scr_msg = "pan";
				redraw();
				break;
			case 'l':
				std::cout << "DRAW_MODE\n";
				m_lm_state = Mode::draw;
				m_md_scr_msg = "draw";
				redraw();
				break;
			case '0':
				std::cout << "ZERO_MODE\n";
				m_lm_state = Mode::select;
				m_md_scr_msg = "default";
				redraw();
				break;
			}
		}
		else if (evt >= 0xa0 && evt <= 0xff) { // ISO-8859-1 (international keyboards)
			char key_buf[8];
			int kl = fl_utf8encode((unsigned)evt, key_buf);
			key_buf[kl] = '\0';
			sprintf_s(buffer, bfsz,"'%d'", key_code);
			ret = 1;
		}
		else if (key_code > FL_F && key_code <= FL_F_Last) {
			sprintf_s(buffer, bfsz,"FL_F+%d", key_code - FL_F);
			ret = 1;
		}
		else if (key_code >= FL_KP && key_code <= FL_KP_Last) {
			sprintf_s(buffer, bfsz, "FL_KP+'%c'", key_code - FL_KP);
			ret = 1;
		}
		else if (key_code >= FL_Button && key_code <= FL_Button + 7) {
			sprintf_s(buffer, "FL_Button+%d", key_code - FL_Button);
			ret = 1;
		}
		std::cout << keyname << '\t' << key_code << '\n';
	}


	switch (evt) {
	case FL_MOVE:
		m_mouse_scr_pos.x = Fl::event_x() - x();
		m_mouse_scr_pos.y = Fl::event_y() - y();
		scr_to_world(Fl::event_x() - x(), Fl::event_y() - y(), m_mouse_world_pos);

        if (world_scale >= 1.0f) {
            snap_mouse_world_pos.x = floorf((m_mouse_world_pos.x + 0.5f) * grid_snap_interval);
            snap_mouse_world_pos.y = floorf((m_mouse_world_pos.y + 0.5f) * grid_snap_interval);
        }

		redraw();
		ret = 1;
		break;
	case FL_PUSH:
		if (Fl::event_button() == FL_MIDDLE_MOUSE) {
			m_wnd->cursor(FL_CURSOR_MOVE);
		}
		drag_sx = (float)Fl::event_x_root();
		drag_sy = (float)Fl::event_y_root();
		is_dragging = true;
		ret = 1;
		break;
	case FL_DRAG: {
		if (is_dragging) {
			if ((Fl::event_button() == FL_MIDDLE_MOUSE) || state.mode == Mode::pan) {
				pan();
			}
			else if (state.mode == Mode::zoom) {
                float update_mouse_x = static_cast<float>(Fl::event_x_root());
                float drag_dist_pix = (update_mouse_x - drag_sx);
                float scale_factor_per_pixel = drag_dist_pix * zooming_sens;
                float scale_factor_percent = scale_factor_per_pixel + 1.0f;

				zoom(ssw / 2, ssh / 2, scale_factor_percent);

                //Printf("%f * %f = %f\n", drag_dist_pix, zooming_sens, scale_factor_percent);
                //
                drag_sx = update_mouse_x;
			}
            else
                redraw();
		}
		ret = 1;
    } break;
	case FL_RELEASE:
		m_wnd->cursor(FL_CURSOR_DEFAULT);
		is_dragging = false;
		ret = 1;
		break;
	case FL_MOUSEWHEEL:
	{
		int mouse_x = Fl::event_x() - x();
		int mouse_y = Fl::event_y() - y();

		int wheel_state = Fl::event_dy();
        float scale_factor_percent = 0.0f;
		if (wheel_state < 1) {
            scale_factor_percent = 1.0f + zooming_factor;
		}
		else if (wheel_state > 0) {
            scale_factor_percent = 1.0f - zooming_factor;
		}

        zoom(mouse_x, mouse_y, scale_factor_percent);

		ret = 1;
	} break;
	default:
		break;
	}

	if (state.mode == Mode::draw) {
        // NOTE(daniel): This part only works for shapes that has two nodes max
		switch (evt) {
		case FL_PUSH:
			if (Fl::event_button() == FL_LEFT_MOUSE) {
                if (state.draw == Draw::line) {
                    temp_shape = new sLine();
                    temp_shape->sinfo = sinfo;
                }
                if (state.draw == Draw::rect) {
                    temp_shape = new sRect();
                    temp_shape->sinfo = sinfo;
                }
                if (state.draw == Draw::circle) {
                    temp_shape = new sCircle();
                    temp_shape->sinfo = sinfo;
                }

				// first node at location of left click
				temp_shape->get_next_node(m_mouse_world_pos);
				m_selected_node = temp_shape->get_next_node(m_mouse_world_pos);

                is_drawing = true;
			}
			ret = 1;
			break;
		case FL_DRAG: {
			m_mouse_scr_pos.x = Fl::event_x() - x();
			m_mouse_scr_pos.y = Fl::event_y() - y();
			scr_to_world(Fl::event_x() - x(), Fl::event_y() - y(), m_mouse_world_pos);
			// second node
			if (m_selected_node != nullptr) {
				m_selected_node->pos = m_mouse_world_pos;
			}
			redraw();
			
			ret = 1;
        } break;
		case FL_RELEASE: {
			if (is_drawing) {
				m_selected_node = temp_shape->get_next_node(m_mouse_world_pos);
				if (m_selected_node == nullptr) {
                    printf("shape push_back\n");
                    shapes.push_back(temp_shape);
                    temp_shape = nullptr;
                }
                else {
                    printf("failed to store shape\n");
                }
                is_drawing = false;
			}
			ret = 1;
        } break;
		}
	}

	return ret;
} // handle member

void View2D::world_to_scr(Vector world, int& scrx, int& scry)
{
	scrx = static_cast<int>((world.x - world_offset.x) * world_scale);
	scry = static_cast<int>((world.y - world_offset.y) * world_scale);
}

void View2D::scr_to_world(int scrx, int scry, Vector& world)
{
	world.x = static_cast<float>(scrx) / world_scale + world_offset.x;
	world.y = static_cast<float>(scry) / world_scale + world_offset.y;
}

void View2D::draw_create_shape()
{
    
}

void View2D::pan()
{
	float update_mouse_x = static_cast<float>(Fl::event_x_root());
	float update_mouse_y = static_cast<float>(Fl::event_y_root());
	// drag difference
	world_offset.x -= (update_mouse_x - drag_sx) / world_scale;
	world_offset.y -= (update_mouse_y - drag_sy) / world_scale;
	drag_sx = update_mouse_x;
	drag_sy = update_mouse_y;
	// BUG
	if (m_drag_constraint) {
		if (world_offset.x < -w()) world_offset.x = static_cast<float>(-w());
		else if (world_offset.x > ssw) world_offset.x = static_cast<float>(ssw);
		if (world_offset.y < -h()) world_offset.y = static_cast<float>(-h());
		else if (world_offset.y > ssh) world_offset.y = static_cast<float>(ssh);
	}
    redraw();
}

void View2D::zoom(int focusx, int focusy, float scale_factor_percent)
{
    Vector bf_center_axis;
	scr_to_world(focusx, focusy, bf_center_axis);

	world_scale *= scale_factor_percent;
    
    if (world_scale < min_zoom) world_scale = min_zoom;
    else if (world_scale > max_zoom) world_scale = max_zoom;

	Vector af_center_axis;
	scr_to_world(focusx, focusy, af_center_axis);

	world_offset.x += (bf_center_axis.x - af_center_axis.x);
	world_offset.y += (bf_center_axis.y - af_center_axis.y);

	redraw();
}

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
    set_menu_items_pointer();
}


void MainWindow::set_menu_items_pointer()
{
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


void save_cb(Fl_Widget* widget, void*)
{
}

void pan_state_cb(Fl_Widget* widget, void*)
{
    Fl_Menu_Item* item = (Fl_Menu_Item*)widget;
    std::cout << item->submenu() << std::endl;
}
void zoom_state_cb(Fl_Widget* widget, void*)
{
}


