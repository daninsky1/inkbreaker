#include "screen_space.h"

ScreenSpace::ScreenSpace(int wdx, int wdy, int wdw, int wdh, Fl_Window* win_wd) :
	Fl_Box{ wdx, wdy, wdw, wdh },
	m_win{ win_wd },
	m_screen_buffer{ 0 },
	m_sspx{ 0 }, m_sspy{ 0 },		// Buffer on top left of the Fl_Box
	m_sspw{ wdw }, m_ssph{ wdh },
	m_buffer_scale{ 0.0f },
	m_scale{ 1.0f },
	m_scale_sens{ 0.1f },
	m_xoff{ 0 }, m_yoff{ 0 },
	m_drag_constraint{ false },
	m_drag_sx{ 0 }, m_drag_sy{ 0 },
	m_drag_state{ true },
	m_ssp_state{ DEFAULT_MODE }
{
	// LOG
	std::cout << "New Offscreen\n";
	static_cast<Fl_Window*>(m_win)->make_current();
	m_screen_buffer = fl_create_offscreen(m_sspw, m_ssph);
	m_buffer_scale = Fl_Graphics_Driver::default_driver().scale();
} // ScreenSpace

void ScreenSpace::draw()
{
	int wdx = x();	// gets the widget x position relative to the window
	int wdy = y();	// gets the widget y position relative to the window
	int wdw = w();	// gets the widget width
	int wdh = h();	// gets the widget height

	// draw background
	fl_color(FL_BLACK);
	fl_rectf(wdx , wdy, wdw, wdh);


	fl_begin_offscreen(m_screen_buffer);
	// BACKGROUND
	fl_color(FL_DARK_RED);
	fl_rectf(m_sspx, m_sspy, m_sspw, m_ssph);
	// DRAW AXES
	float axx = 0.0f;
	float axy = 0.0f;
	int waxx, waxy;
	world_to_scr(axx, axy, waxx, waxy);
	fl_line_style(FL_SOLID, 1.0f);
	fl_color(FL_RED);
	fl_line(0, waxy, w(), waxy);
	fl_color(FL_GREEN);
	fl_line(waxx, 0, waxx, h());

	
	fl_color(FL_WHITE);
	fl_font(FL_TIMES_BOLD_ITALIC, 50.0f*m_scale);

	float tx = static_cast<float>(w() / 2);
	float ty = static_cast<float>(h() / 2);
	int stx, sty;
	world_to_scr(tx, ty, stx, sty);
	fl_draw("TE AMO !!!", stx, sty);

	// GRID
	// TODO: IMPLEMENT GRID OBJECT
	fl_line_style(FL_SOLID, 2.0f*m_scale);
	// horizontal lines
	float gridx = 100.0f, gridy = 100.0f;
	for (int y = 0.0f; y <= gridy; y+=gridy/10.0f) {
		int startx = 0.0f, starty = y;
		int endx = gridx, endy = y;

		// world to screen
		int start_sspx, start_sspy, end_sspx, end_sspy;
		world_to_scr(startx, starty, start_sspx, start_sspy);
		world_to_scr(endx, endy, end_sspx, end_sspy);

		fl_line(start_sspx, start_sspy, end_sspx, end_sspy);
	}

	//// vertical lines
	for (int x = 0.0f; x <= gridx; x+=gridx/10.0f) {
		int startx = x, starty = 0.0f;
		int endx = x, endy = gridy;

		// world to screen
		int start_sspx, start_sspy, end_sspx, end_sspy;
		world_to_scr(startx, starty, start_sspx, start_sspy);
		world_to_scr(endx, endy, end_sspx, end_sspy);

		fl_line(start_sspx, start_sspy, end_sspx, end_sspy);
	}



	fl_end_offscreen();

	if (m_buffer_scale != Fl_Graphics_Driver::default_driver().scale()) {
		fl_rescale_offscreen(m_screen_buffer);
		m_buffer_scale = Fl_Graphics_Driver::default_driver().scale();
	}
	fl_copy_offscreen(wdx, wdy, wdw, wdh, m_screen_buffer, 0, 0);

	// LOG
	fl_color(FL_WHITE);
	int font = FL_COURIER;
	int font_sz = 14;
	fl_font(font, 14);
	char c_draw_log_msg[100];
	sprintf(c_draw_log_msg,
		"ScreenSpace Size: (%d x %d)", m_ssph, m_sspw);
	fl_draw(c_draw_log_msg, wdx, wdy + fl_height(font, font_sz));
	sprintf(c_draw_log_msg,
		"World Offset: (%f, %f)",
		m_xoff, m_yoff);
	fl_draw(c_draw_log_msg, wdx, wdy + fl_height(font, font_sz) * 2);
	sprintf(c_draw_log_msg,
		"Scale Factor: %f",
		m_scale);
	fl_draw(c_draw_log_msg, wdx, wdy + fl_height(font, font_sz) * 3);
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
		char buffer[100];
		const char* keyname = buffer;
		key_code = Fl::event_key();
		// ONLY HANDLES ASCII EVENT KEYS !!!!!!!
		if (key_code < 128) { // ASCII
			sprintf(buffer, "'%c'", key_code);
			switch (key_code) {
			case 'z':
				std::cout << "ZOOM_MODE\n";
				m_ssp_state = ZOOM_MODE;
				break;
			case 'h': case ' ':
				std::cout << "DRAG_MODE\n";
				m_ssp_state = PANNING_MODE;
				break;
			case '0':
				std::cout << "ZERO_MODE\n";
				m_ssp_state = DEFAULT_MODE;
				break;
			}
		}
		else if (evt >= 0xa0 && evt <= 0xff) { // ISO-8859-1 (international keyboards)
			char key_buf[8];
			int kl = fl_utf8encode((unsigned)evt, key_buf);
			key_buf[kl] = '\0';
			sprintf(buffer, "'%s'", key_code);
			ret = 1;
		}
		else if (key_code > FL_F && key_code <= FL_F_Last) {
			sprintf(buffer, "FL_F+%d", key_code - FL_F);
			ret = 1;
		}
		else if (key_code >= FL_KP && key_code <= FL_KP_Last) {
			sprintf(buffer, "FL_KP+'%c'", key_code - FL_KP);
			ret = 1;
		}
		else if (key_code >= FL_Button && key_code <= FL_Button + 7) {
			sprintf(buffer, "FL_Button+%d", key_code - FL_Button);
			ret = 1;
		}
		std::cout << keyname << '\t' << key_code << '\n';
	}

	int mouse_x, mouse_y;	// mouse position

	switch (evt) {
	case FL_ENTER:
		static_cast<Fl_Window*>(m_win)->cursor(FL_CURSOR_MOVE);
		ret = 1;
		break;
	case FL_LEAVE:
		static_cast<Fl_Window*>(m_win)->cursor(FL_CURSOR_DEFAULT);
		ret = 1;
		break;
	case FL_PUSH:
		m_drag_sx = Fl::event_x_root();
		m_drag_sy = Fl::event_y_root();
		m_drag_state = true;
		ret = 1;
		break;
	case FL_DRAG:
		if (m_drag_state) {
			pan();
			redraw();
		}
		ret = 1;
		break;
	case FL_RELEASE:
		m_drag_state = false;
		ret = 1;
		break;
	case FL_MOUSEWHEEL:
	{
		int mouse_x = Fl::event_x() - x();
		int mouse_y = Fl::event_y() - y();
		std::cout << mouse_x << ", " << mouse_y << '\n';

		float mouse_bfz_worldx, mouse_bfz_worldy;		// mouse coordinates on the world before zoom
		scr_to_world(mouse_x, mouse_y, mouse_bfz_worldx, mouse_bfz_worldy);

		int wheel_state = Fl::event_dy();
		if (wheel_state < 1) {
			m_scale *= (1.0f + m_scale_sens);
			redraw();
		}
		else if (wheel_state > 0) {
			m_scale *= (1.0f - m_scale_sens);
			redraw();
		}

		float mouse_afz_worldx, mouse_afz_worldy;		// mouse coordinates on the world after zoom
		scr_to_world(mouse_x, mouse_y, mouse_afz_worldx, mouse_afz_worldy);

		m_xoff += (mouse_bfz_worldx - mouse_afz_worldx);
		m_yoff += (mouse_bfz_worldy - mouse_afz_worldy);
		ret = 1;
		break;
	}
	default:
		break;
	}
	return ret;
} // handle member

void ScreenSpace::world_to_scr(float worldx, float worldy, int& scrx, int& scry)
{
	scrx = static_cast<int>((worldx - m_xoff) * m_scale);
	scry = static_cast<int>((worldy - m_yoff) * m_scale);
}

void ScreenSpace::scr_to_world(int scrx, int scry, float& worldx, float& worldy)
{
	worldx = static_cast<float>(scrx) / m_scale + m_xoff;
	worldy = static_cast<float>(scry) / m_scale + m_yoff;
}

void ScreenSpace::pan()
{
	float update_mouse_x = static_cast<float>(Fl::event_x_root());
	float update_mouse_y = static_cast<float>(Fl::event_y_root());
	// drag difference
	m_xoff -= (update_mouse_x - m_drag_sx) / m_scale;
	m_yoff -= (update_mouse_y - m_drag_sy) / m_scale;
	m_drag_sx = update_mouse_x;
	m_drag_sy = update_mouse_y;
	// BUG
	if (m_drag_constraint) {
		if (m_xoff < -w()) m_xoff = -w();
		else if (m_xoff > m_sspw) m_xoff = m_sspw;
		if (m_yoff < -h()) m_yoff = -h();
		else if (m_yoff > m_ssph) m_yoff = m_ssph;
	}
}

void ScreenSpace::zoom()
{
}

