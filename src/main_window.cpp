#include "InkBreakerConfig.h"

#include "main_window.h"

void line_state_cb(Fl_Widget* widget, void*) {}

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

void clear_cb(Fl_Widget* widget, void*)
{

}
void pan_state_cb(Fl_Widget* widget, void*);
void zoom_state_cb(Fl_Widget* widget, void*);


Fl_Menu_Item menutable[] = {
    { "INKBREAKER", 0, (Fl_Callback*)about_cb, nullptr, FL_MENU_INACTIVE },
    { "&File",      0, nullptr, nullptr, FL_SUBMENU },
        { "&New",        FL_COMMAND + 'n', (Fl_Callback*)new_cb },
        { "&Save",       FL_COMMAND + 's', (Fl_Callback*)save_cb },
        { "&Save as...", FL_COMMAND + FL_ALT + 's', (Fl_Callback*)saveas_cb },
        { "Quit", FL_COMMAND + 'q', (Fl_Callback*)quit_cb },
    { 0 },
    // TODO: Implement procedural texture generation after deployment to image support
    /*
    { "Generators", 0, nullptr, nullptr, FL_SUBMENU },
        { "Blank", 0, (Fl_Callback*)blank_cb},
        { "Line Splash Animation", 0, (Fl_Callback*)perlin_noise_anim_cb },
        { "Perlin Noise Animation", 0, (Fl_Callback*)perlin_noise_anim_cb },
        { 0 },
    */
    { "State", 0, nullptr, nullptr, FL_SUBMENU },
        { "Pan", 0, (Fl_Callback*)pan_state_cb},
        { "Zoom", 0, (Fl_Callback*)zoom_state_cb},
        { "Draw", 0, nullptr, nullptr, FL_SUBMENU},
            { "Line", 0, (Fl_Callback*)line_state_cb},
            { 0 },
        { 0 },
    { "&Help", 0, nullptr, nullptr, FL_SUBMENU },
        { "Controls", 0, (Fl_Callback*)controls_cd },
        { "&About", 0, (Fl_Callback*)about_cb },
        { 0 },
    { 0 }
};

ScreenSpace::ScreenSpace(int wdx, int wdy, int wdw, int wdh, Fl_Double_Window* win) :
	Fl_Box{ wdx, wdy, wdw, wdh },
	m_win{ win },
	m_sspw{ wdw }, m_ssph{ wdh }
{
	// LOG
	std::cout << "Create Offscreen\n";
	m_screen_buffer = fl_create_offscreen(m_sspw, m_ssph);
	m_buffer_scale = Fl_Graphics_Driver::default_driver().scale();
} // ScreenSpace

void ScreenSpace::draw()
{
	int wdx = x();	// gets the widget x position relative to the window
	int wdy = y();	// gets the widget y position relative to the window
	int wdw = w();	// gets the widget width
	int wdh = h();	// gets the widget height

	// DRAW WIDGET BACKGROUND
	fl_color(FL_BLACK);
	fl_rectf(wdx , wdy, wdw, wdh);
	// DRAW BACKGROUND
	fl_begin_offscreen(m_screen_buffer);
	fl_color(FL_DARK_RED);
	fl_rectf(m_sspx, m_sspy, m_sspw, m_ssph);
	// DRAW GRID
	// TODO: IMPLEMENT GRID


	// DRAW AXES
	Vector ax{ 1.0, 1.0 };
	int waxx, waxy;
	world_to_scr(ax, waxx, waxy);
	fl_line_style(FL_SOLID, 1);
	fl_color(FL_RED);
	fl_line(0, waxy, w(), waxy);
	fl_color(FL_GREEN);
	fl_line(waxx, 0, waxx, h());

	// MY LINE
	sShape::world_offset = m_off;
	sShape::world_scale = m_scale;
	/* HARDCODED sLine
	m_line = new sLine();
	m_line->get_next_node(Vector{ 0.0, 0.0 });
	m_line->get_next_node(Vector{ 100.0, 100.0 });
	*/
	if (!m_shapes.empty()) {
		for (auto& shape : m_shapes) {
			shape->draw_shape();
			//shape->draw_nodes();
		}
	}

	if (m_temp_shape) {
		m_temp_shape->draw_shape();
		//m_temp_shape->draw_nodes();
	}

	// TODO: A RENDER QUEUE here
	fl_color(FL_WHITE);
	fl_line_style(FL_SOLID, 2*(int)m_scale);
	fl_begin_line();
	for (float i = 0; i < w(); i += 0.1f) {
		Vector wv{ i, std::sin(i * (0.5 * 0.1)) * 50 };	// world vector
		int sx, sy;
		world_to_scr(wv, sx, sy);
		
		fl_vertex(sx, sy);
	}
	fl_end_line();

	/* TE AMO MSG
	fl_font(FL_TIMES_BOLD_ITALIC, 50.0f*m_scale);

	float tx = static_cast<float>(w() / 2);
	float ty = static_cast<float>(h() / 2);
	int stx, sty;
	world_to_scr(tx, ty, stx, sty);
	fl_draw("TE AMO !!!", stx, sty);
	*/

	// GRID
	// TODO: IMPLEMENT GRID OBJECT
	/*
	* 
	fl_line_style(FL_SOLID, 2.0f*m_scale);
	// horizontal lines
	float gridx = 100.0f, gridy = 100.0f;
	for (double y = 0.0; y <= gridy; y+=gridy/10.0) {
		Vector start{ 0.0, y };
		Vector end{ gridx, y };

		// world to screen
		int start_sspx, start_sspy, end_sspx, end_sspy;
		world_to_scr(start, start_sspx, start_sspy);
		world_to_scr(end, end_sspx, end_sspy);

		fl_line(start_sspx, start_sspy, end_sspx, end_sspy);
	}

	//// vertical lines
	for (double x = 0.0; x <= gridx; x+=gridx/10.0) {
		Vector start{ x, 0.0 };
		Vector end{ x, gridy };
		int startx = x, starty = 0.0f;
		int endx = x, endy = gridy;

		// world to screen
		int start_sspx, start_sspy, end_sspx, end_sspy;
		world_to_scr(start, start_sspx, start_sspy);
		world_to_scr(end, end_sspx, end_sspy);

		fl_line(start_sspx, start_sspy, end_sspx, end_sspy);
	}
	*/

	fl_end_offscreen();

	if (m_buffer_scale != Fl_Graphics_Driver::default_driver().scale()) {
		fl_rescale_offscreen(m_screen_buffer);
		m_buffer_scale = Fl_Graphics_Driver::default_driver().scale();
	}
	fl_copy_offscreen(wdx, wdy, wdw, wdh, m_screen_buffer, 0, 0);

	fl_color(FL_WHITE);
	int font = FL_COURIER;
	int font_sz = 14;
	fl_font(font, 15);

	// SCREEN MESSAGENS
	// mode
	int pad = 10;
	fl_draw(m_md_scr_msg.c_str(), wdx+pad, h()+y() - pad - fl_height(font, font_sz)*2);
	// screen mouse position
	std::string mouse_coor;
	mouse_coor.append("mouse screen x: ");
	mouse_coor.append(std::to_string(m_mouse_scr_pos.x));
	mouse_coor.append(" y: ");
	mouse_coor.append(std::to_string(m_mouse_scr_pos.y));
	fl_draw(mouse_coor.c_str(), wdx + pad, h() + y() - pad - fl_height(font, font_sz));
	// world mouse position
	mouse_coor = "";
	mouse_coor.append("mouse world x: ");
	mouse_coor.append(std::to_string(m_mouse_world_pos.x));
	mouse_coor.append(" y: ");
	mouse_coor.append(std::to_string(m_mouse_world_pos.y));
	fl_draw(mouse_coor.c_str(), wdx + pad, h() + y() - pad);

	// LOG
	std::stringstream ss_log;
	ss_log << std::fixed;
	ss_log << "ScreenSpace Size: " << '(' << m_ssph << " x " << m_sspw << ')';
	fl_draw(ss_log.str().c_str(), wdx, wdy + fl_height(font, font_sz));
	ss_log.str(std::string{""});
	ss_log.str("");
	ss_log << "World Offset: " << '(' << m_off.x << " x " << m_off.y << ')';
	fl_draw(ss_log.str().c_str(), wdx, wdy + fl_height(font, font_sz) * 2);
	ss_log.str("");
	ss_log << "Scale Factor:" << m_scale;
	fl_draw(ss_log.str().c_str(), wdx, wdy + fl_height(font, font_sz) * 3);
}

int ScreenSpace::handle(int evt)
{
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
			case 'z':
				std::cout << "ZOOM_MODE\n";
				m_lm_state = mode::zoom;
				m_md_scr_msg = "zoom";
				redraw();
				break;
			case 'h': case ' ':
				std::cout << "DRAG_MODE\n";
				m_lm_state = mode::pan;
				m_md_scr_msg = "pan";
				redraw();
				break;
			case 'l':
				std::cout << "DRAW_MODE\n";
				m_lm_state = mode::draw;
				m_md_scr_msg = "draw";
				redraw();
				break;
			case '0':
				std::cout << "ZERO_MODE\n";
				m_lm_state = mode::default;
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
		redraw();
		ret = 1;
		break;
	case FL_PUSH:
		if (Fl::event_button() == FL_MIDDLE_MOUSE) {
			m_win->cursor(FL_CURSOR_MOVE);
		}
		m_drag_sx = (float)Fl::event_x_root();
		m_drag_sy = (float)Fl::event_y_root();
		m_drag_state = true;
		ret = 1;
		break;
	case FL_DRAG:
		if (m_drag_state) {
			if ((Fl::event_button() == FL_MIDDLE_MOUSE) || m_lm_state == mode::pan) {
				pan();
			}
			else if (m_lm_state == mode::zoom) {
				zoom();
			}
			else if (m_lm_state == mode::draw) {
				draw_line();
			}
			redraw();
		}
		ret = 1;
		break;
	case FL_RELEASE:
		m_win->cursor(FL_CURSOR_DEFAULT);
		m_drag_state = false;
		ret = 1;
		break;
	case FL_MOUSEWHEEL:
	{
		int mouse_x = Fl::event_x() - x();
		int mouse_y = Fl::event_y() - y();

		Vector mouse_bf_world;		// mouse coordinates on the world before zoom	
		scr_to_world(mouse_x, mouse_y, mouse_bf_world);

		int wheel_state = Fl::event_dy();
		if (wheel_state < 1) {
			m_scale *= (1.0f + m_scale_sens);
			redraw();
		}
		else if (wheel_state > 0) {
			m_scale *= (1.0f - m_scale_sens);
			redraw();
		}

		Vector mouse_af_world;
		scr_to_world(mouse_x, mouse_y, mouse_af_world);

		m_off.x += (mouse_bf_world.x - mouse_af_world.x);
		m_off.y += (mouse_bf_world.y - mouse_af_world.y);
		ret = 1;
		break;
	}
	default:
		break;
	}

	if (m_lm_state == mode::draw) {
		switch (evt) {
		case FL_PUSH:
			if (Fl::event_button() == FL_LEFT_MOUSE) {
				m_temp_shape = new sRect();

				// first node at location of left click
				m_temp_shape->get_next_node(m_mouse_world_pos);
				m_selected_node = m_temp_shape->get_next_node(m_mouse_world_pos);
			}
			ret = 1;
			break;
		case FL_DRAG:
			m_mouse_scr_pos.x = Fl::event_x() - x();
			m_mouse_scr_pos.y = Fl::event_y() - y();
			scr_to_world(Fl::event_x() - x(), Fl::event_y() - y(), m_mouse_world_pos);
			// second node
			if (m_selected_node != nullptr) {
				m_selected_node->pos = m_mouse_world_pos;
			}
			redraw();
			
			ret = 1;
			break;
		case FL_RELEASE:
			if (m_temp_shape) {
				m_selected_node = m_temp_shape->get_next_node(m_mouse_world_pos);
				if (m_selected_node == nullptr) m_shapes.push_back(m_temp_shape);
			}
			ret = 1;
			break;
		}
	}

	return ret;
} // handle member

void ScreenSpace::world_to_scr(Vector world, int& scrx, int& scry)
{
	scrx = static_cast<int>((world.x - m_off.x) * m_scale);
	scry = static_cast<int>((world.y - m_off.y) * m_scale);
}

void ScreenSpace::scr_to_world(int scrx, int scry, Vector& world)
{
	world.x = static_cast<float>(scrx) / m_scale + m_off.x;
	world.y = static_cast<float>(scry) / m_scale + m_off.y;
}

void ScreenSpace::pan()
{
	float update_mouse_x = static_cast<float>(Fl::event_x_root());
	float update_mouse_y = static_cast<float>(Fl::event_y_root());
	// drag difference
	m_off.x -= (update_mouse_x - m_drag_sx) / m_scale;
	m_off.y -= (update_mouse_y - m_drag_sy) / m_scale;
	m_drag_sx = update_mouse_x;
	m_drag_sy = update_mouse_y;
	// BUG
	if (m_drag_constraint) {
		if (m_off.x < -w()) m_off.x = -w();
		else if (m_off.x > m_sspw) m_off.x = m_sspw;
		if (m_off.y < -h()) m_off.y = -h();
		else if (m_off.y > m_ssph) m_off.y = m_ssph;
	}
}

void ScreenSpace::zoom()
{
	int sx = 340;
	int sy = 180;
	Vector bf_center_axis;
	scr_to_world(sx, sy, bf_center_axis);

	// TODO: bug, scale according to percentage
	float update_mouse_x = static_cast<float>(Fl::event_x_root());
	m_scale += (update_mouse_x - m_drag_sx) * (m_scale_sens / 20.0f);
	m_drag_sx = update_mouse_x;
	redraw();

	Vector af_center_axis;
	scr_to_world(sx, sy, af_center_axis);

	m_off.x += (bf_center_axis.x - af_center_axis.x);
	m_off.y += (bf_center_axis.y - af_center_axis.y);

	redraw();
}

void ScreenSpace::draw_line()
{

}


constexpr int MENUBAR_H = 30;

MainWindow::MainWindow(int sspw, int ssph, const char* l) :
    Fl_Double_Window{ sspw, MENUBAR_H+ssph, l },
    m_mbar{0, 0, sspw, 30},
    m_ssp{ new ScreenSpace{0, MENUBAR_H, sspw, ssph, this} }
{
    make_current();
    m_mbar.menu(menutable);
    add(m_mbar);
    add(m_ssp);
} // MainWindow

MainWindow::~MainWindow()
{
    delete m_ssp;
}

void pan_state_cb(Fl_Widget* widget, void*)
{
    Fl_Menu_Item* item = (Fl_Menu_Item*)widget;
    std::cout << item->submenu() << std::endl;
}
void zoom_state_cb(Fl_Widget* widget, void*)
{
}
