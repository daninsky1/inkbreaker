#include "view2d.h"

constexpr int SELECTION_THRESHOLD = 5;

View2D::View2D(int x, int y, int w, int h, std::vector<Shape*> &p_shapes) :
	Fl_Box{ x, y, w, h },
    shapes { p_shapes }
{
    scr_buf = fl_create_offscreen(w, h);
    fl_offscr_scale = Fl_Graphics_Driver::default_driver().scale();

    v2d_x = x; v2d_y = y;
    v2d_w = w; v2d_h = h;
} // View2D

void View2D::draw()
{
    // NOTE(daniel): The drawing functions only works in a limited way, you can
    // only draw on the window coordinates, any other way to control some buffer
    // outside this coordinates in painful or show some artefacts and will not
    // be implement for now, that said the only way is to pass the View2D
    // widget offsets to every fltk draw function calls.
    
    if (v2d_w != w() || v2d_h != h()) {
        v2d_x = x();
        v2d_y = y();
        v2d_w = w();
        v2d_h = h();
        fl_delete_offscreen(scr_buf);
        scr_buf = fl_create_offscreen(v2d_w, v2d_h);
    }
    if (fl_offscr_scale != Fl_Graphics_Driver::default_driver().scale()) {
        // the screen scaling factor has changed
        fl_rescale_offscreen(scr_buf);
        fl_offscr_scale = Fl_Graphics_Driver::default_driver().scale();
    }

    // NOTE(daniel): This need to be updated manually, until we find a better
    // way to do it
	Shape::world_offset = world_offset;
	Shape::world_scale = world_scale;

	// DRAW WORLD BACKGROUND
    fl_begin_offscreen(scr_buf);
	fl_color(FL_DARK3);
	fl_rectf(0, 0, v2d_w, v2d_h);
	// DRAW AXES
	int axx, axy;
	world_to_scr(axes_center, axx, axy);
    View2D::draw_axes(axx, axy, v2d_w, v2d_w, 1);
    // DRAW GRID
    if (show_grid && (world_scale >= 15.0f)) {
        // Draw grid
        draw_grid(5);
    }
    // NOTE(daniel): Render queue
    for (int i = 0; i < shapes.size(); ++i) {
        shapes[i]->draw_shape();
    }

	if (temp_shape) {
		temp_shape->draw_shape();
		//temp_shape->draw_nodes();
	}

    if (is_selecting) {
        select_box->draw_shape();
    }

    if (app_state->active_selection) {
        app_state->active_selection->draw_bbox();
    }

    // DRAW SNAP CURSOR
    Vector2f scr_origin_to_world;
    scr_to_world(0, 0, scr_origin_to_world);
    Vector2f scr_size_to_world;
    scr_to_world(v2d_w, v2d_h, scr_size_to_world);

    if (world_scale >= 15.0f) {
        fl_line_style(FL_SOLID, 1);
        fl_color(FL_YELLOW);
        fl_circle(snap_cursor_v2d.x, snap_cursor_v2d.y, 4);
    }

	// SCREEN DEBUG MESSAGES
	fl_color(FL_WHITE);
	int font = FL_COURIER;
	int font_sz = 14;
	fl_font(font, 15);

	int pad = 10;

    const int msg_sz = 128;
    char scr_dmsg[msg_sz];
    sprintf_s(scr_dmsg, msg_sz, "View2D: (%d, %d)", v2d_w, v2d_h);
	fl_draw(scr_dmsg, v2d_x + pad, fl_height(font, font_sz));
    sprintf_s(scr_dmsg, msg_sz, "World offset: (%.3f, %.3f)", world_offset.x, world_offset.y);
	fl_draw(scr_dmsg, v2d_x + pad, fl_height(font, font_sz) * 2);
    sprintf_s(scr_dmsg, msg_sz, "World scale: %.3f", world_scale);
	fl_draw(scr_dmsg, v2d_x + pad, fl_height(font, font_sz) * 3);

	// mode
    //fl_draw(m_md_scr_msg.c_str(), v2d_x+pad, h() - pad - fl_height(font, font_sz)*4);

    sprintf_s(scr_dmsg, msg_sz, "mouse world: (%.3f, %.3f)", mouse_world.x, mouse_world.y);
	fl_draw(scr_dmsg, v2d_x + pad, h() - pad - fl_height(font, font_sz)*3);
    sprintf_s(scr_dmsg, msg_sz, "mouse world snap: (%.3f, %.3f)", mouse_snap_world.x, mouse_snap_world.y);
	fl_draw(scr_dmsg, v2d_x + pad, v2d_h - pad - fl_height(font, font_sz)*2);
    sprintf_s(scr_dmsg, msg_sz, "mouse view2d: (%d, %d)", mouse_v2d.x, mouse_v2d.y);
	fl_draw(scr_dmsg, v2d_x + pad, v2d_h - pad - fl_height(font, font_sz));
    sprintf_s(scr_dmsg, msg_sz, "mouse view2d snap: (%d, %d)", snap_cursor_v2d.x, snap_cursor_v2d.y);
	fl_draw(scr_dmsg, v2d_x + pad, v2d_h - pad);

    fl_end_offscreen();
    fl_copy_offscreen(v2d_x, v2d_y, v2d_w, v2d_h, scr_buf, 0, 0);
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
	//int key_code = 0;
	//if (evt == FL_KEYBOARD) {
    //    constexpr int bfsz = 100;
	//	char buffer[bfsz];
	//	const char* keyname = buffer;
	//	key_code = Fl::event_key();
	//	// ONLY HANDLES ASCII EVENT KEYS !!!!!!!
	//	if (key_code < 128) { // ASCII
	//		sprintf_s(buffer, bfsz,"'%c'", key_code);
	//		switch (key_code) {
	//		case 'r': {
	//			std::cout << v2d_w << ", " << v2d_h << " " << w() << ", " << h() << std::endl;
	//			redraw();
	//		} break;
	//		case 'z':
	//			std::cout << "ZOOM_MODE\n";
	//			m_lm_state = Mode::zoom;
	//			m_md_scr_msg = "zoom";
	//			redraw();
	//			break;
	//		case 'h': case ' ':
	//			std::cout << "DRAG_MODE\n";
	//			m_lm_state = Mode::pan;
	//			m_md_scr_msg = "pan";
	//			redraw();
	//			break;
	//		case 'l':
	//			std::cout << "DRAW_MODE\n";
	//			m_lm_state = Mode::draw;
	//			m_md_scr_msg = "draw";
	//			redraw();
	//			break;
	//		case '0':
	//			std::cout << "ZERO_MODE\n";
	//			m_lm_state = Mode::select;
	//			m_md_scr_msg = "default";
	//			redraw();
	//			break;
	//		}
	//	}
	//	else if (evt >= 0xa0 && evt <= 0xff) { // ISO-8859-1 (international keyboards)
	//		char key_buf[8];
	//		int kl = fl_utf8encode((unsigned)evt, key_buf);
	//		key_buf[kl] = '\0';
	//		sprintf_s(buffer, bfsz,"'%d'", key_code);
	//		ret = 1;
	//	}
	//	else if (key_code > FL_F && key_code <= FL_F_Last) {
	//		sprintf_s(buffer, bfsz,"FL_F+%d", key_code - FL_F);
	//		ret = 1;
	//	}
	//	else if (key_code >= FL_KP && key_code <= FL_KP_Last) {
	//		sprintf_s(buffer, bfsz, "FL_KP+'%c'", key_code - FL_KP);
	//		ret = 1;
	//	}
	//	else if (key_code >= FL_Button && key_code <= FL_Button + 7) {
	//		sprintf_s(buffer, "FL_Button+%d", key_code - FL_Button);
	//		ret = 1;
	//	}
	//	std::cout << keyname << '\t' << key_code << '\n';
	//}


	switch (evt) {
	case FL_MOVE: {
        get_cursor_v2d_position(mouse_v2d.x, mouse_v2d.y);
		scr_to_world(mouse_v2d.x, mouse_v2d.y, mouse_world);
        mouse_snap_world = get_snap_grid(mouse_world);
        world_to_scr(mouse_snap_world, snap_cursor_v2d.x, snap_cursor_v2d.y);

		redraw();
		ret = 1;
    } break;
	case FL_PUSH:
		if (Fl::event_button() == FL_MIDDLE_MOUSE) {
            change_cursor(FL_CURSOR_MOVE);
		}
        get_cursor_v2d_position(mouse_v2d.x, mouse_v2d.y);
        scr_to_world(mouse_v2d.x, mouse_v2d.y, drag_start_world);
        drag_start_world = get_snap_grid(drag_start_world);
		drag_sx = (float)Fl::event_x_root();
		drag_sy = (float)Fl::event_y_root();
		drag_sxi = Fl::event_x_root();
		drag_syi = Fl::event_y_root();
		is_dragging = true;
		ret = 1;
		break;
	case FL_DRAG: {
        get_cursor_v2d_position(mouse_v2d.x, mouse_v2d.y);
		scr_to_world(mouse_v2d.x, mouse_v2d.y, mouse_world);
        mouse_snap_world = get_snap_grid(mouse_world);
        world_to_scr(mouse_snap_world, snap_cursor_v2d.x, snap_cursor_v2d.y);

		if (is_dragging) {
            int drag_update_x = Fl::event_x_root();
            int drag_update_y = Fl::event_y_root();
			if ((Fl::event_button() == FL_MIDDLE_MOUSE) || (state.mode == Mode::pan && Fl::event_button() == FL_LEFT_MOUSE)) {
				pan(drag_update_x - drag_sxi, drag_update_y - drag_syi);
			}
			else if (state.mode == Mode::zoom && Fl::event_button() == FL_LEFT_MOUSE) {
                float drag_dist_pix = static_cast<float>(drag_update_x - drag_sxi);
                float scale_factor_per_pixel = drag_dist_pix * zooming_sens;
                float scale_factor_percent = scale_factor_per_pixel + 1.0f;

                // NOTE(daniel): Maybe use drag start to center zoom
				zoom(scale_factor_percent, v2d_w / 2, v2d_h / 2);

                world_to_scr(drag_start_world, snap_cursor_v2d.x, snap_cursor_v2d.y);

                //Printf("%f * %f = %f\n", drag_dist_pix, zooming_sens, scale_factor_percent);
			}
            drag_sxi = drag_update_x;
            drag_syi = drag_update_y;
		}
        redraw();
		ret = 1;
    } break;
	case FL_RELEASE:
        change_cursor(FL_CURSOR_DEFAULT);
		is_dragging = false;
		ret = 1;
		break;
	case FL_MOUSEWHEEL: {
        get_cursor_v2d_position(mouse_v2d.x, mouse_v2d.y);
		scr_to_world(mouse_v2d.x, mouse_v2d.y, mouse_world);

		int wheel_state = Fl::event_dy();
        float scale_factor_percent = 0.0f;
		if (wheel_state < 1) {
            scale_factor_percent = 1.0f + zooming_factor;
		}
		else if (wheel_state > 0) {
            scale_factor_percent = 1.0f - zooming_factor;
		}

        zoom(scale_factor_percent, mouse_v2d.x, mouse_v2d.y);

        mouse_snap_world = get_snap_grid(mouse_world);
        world_to_scr(mouse_snap_world, snap_cursor_v2d.x, snap_cursor_v2d.y);
		ret = 1;
	} break;
	default:
		break;
	}

	if (state.mode == Mode::select) {
		switch (evt) {
		case FL_PUSH: {
            get_cursor_v2d_position(mouse_v2d.x, mouse_v2d.y);
			if (Fl::event_button() == FL_LEFT_MOUSE) {
                if (!select_box) {
                    select_box = new SelectBox();
                    // first node at location of left click
                    select_box->get_next_node(mouse_world);
                    select_box->get_next_node(mouse_world);
                }
                else {
                    select_box->nodes[0].pos = mouse_world;
                }
			}
            is_selecting = true;
            if (app_state->active_selection) {
                if (app_state->active_selection->is_inside_bbox(mouse_world)) {
                    drag_start_world = mouse_world;
                    is_selecting = false;
                    is_moving = true;
                    changed = true;
                }
            }
			ret = 1;
        } break;
		case FL_DRAG: {
            get_cursor_v2d_position(mouse_v2d.x, mouse_v2d.y);
			scr_to_world(mouse_v2d.x, mouse_v2d.y, mouse_world);

            if (is_selecting) {
                // second node
                select_box->nodes[1].pos = mouse_world;
            }
            else if (is_moving) {
                for (int i = 0; i < app_state->active_selection->nodes.size(); ++i) {
                    app_state->active_selection->nodes[i].pos.x += (mouse_world.x - drag_start_world.x);
                    app_state->active_selection->nodes[i].pos.y += (mouse_world.y - drag_start_world.y);
                }
                drag_start_world.x = mouse_world.x;
                drag_start_world.y = mouse_world.y;
                app_state->active_selection->update_bbox();
                //printf("drag world distance: %f, %f\n", (drag_start_world.x - mouse_world.x), (drag_start_world.y - mouse_world.y));
                //printf("%s position: %f, %f - %f, %f\n", active_selection->type().c_str(), active_selection->nodes[0].pos.x, active_selection->nodes[0].pos.y, active_selection->nodes[1].pos.x, active_selection->nodes[1].pos.y);
            }
            redraw();
			ret = 1;
        } break;
		case FL_RELEASE: {
            // TODO(daniel): Finish and store selection
            if (is_selecting) {
                if (select_box->scrw > SELECTION_THRESHOLD || select_box->scrh > SELECTION_THRESHOLD) {
                    // Do box selection
                }
                else {
                    // Do click seletion
                    scr_to_world(Fl::event_x() - x(), Fl::event_y() - y(), mouse_world);
                    for (std::vector<Shape*>::reverse_iterator it = shapes.rbegin(); it < shapes.rend(); ++it) {
                        if ((*it)->is_inside_bbox(mouse_world)) {
                            *it = app_state->active_selection;
                            app_state->active_selection = *it;
                            printf("%s position: %f, %f\n", app_state->active_selection->type().c_str(), app_state->active_selection->nodes[0].pos.x, app_state->active_selection->nodes[0].pos.y);
                            break;
                            }
                        app_state->active_selection = nullptr;
                        //(*it)->update_bbox();
                        //if (((mouse_world.x > (*it)->bboxs.x) && (mouse_world.x < (*it)->bboxe.x))
                        //    && ((mouse_world.y > (*it)->bboxs.y) && (mouse_world.x < (*it)->bboxe.y))) {
                        //}
                    }
                }
            }
            else if (is_moving) {
                printf("stop moving\n");
                is_moving = false;
            }

            is_selecting = false;
            redraw();
            
			ret = 1;
        } break;
		}
	}

    else if (state.mode == Mode::draw) {
        // NOTE(daniel): This part only works for shapes that has two nodes max
		switch (evt) {
		case FL_PUSH:
			if (Fl::event_button() == FL_LEFT_MOUSE) {
                if (state.draw == Draw::line) {
                    temp_shape = new Line();
                    temp_shape->sinfo = sinfo;
                }
                if (state.draw == Draw::rect) {
                    temp_shape = new Rect();
                    temp_shape->sinfo = sinfo;
                }
                if (state.draw == Draw::circle) {
                    temp_shape = new Circle();
                    temp_shape->sinfo = sinfo;
                }

				// first node at location of left click
				temp_shape->get_next_node(mouse_world);
				m_selected_node = temp_shape->get_next_node(mouse_world);

                is_drawing = true;
			}
			ret = 1;
			break;
		case FL_DRAG: {
            get_cursor_v2d_position(mouse_v2d.x, mouse_v2d.y);
			scr_to_world(Fl::event_x() - x(), Fl::event_y() - y(), mouse_world);
			// second node
			if (m_selected_node != nullptr) {
				m_selected_node->pos = mouse_world;
			}
			redraw();
			
			ret = 1;
        } break;
		case FL_RELEASE: {
			if (is_drawing) {
				m_selected_node = temp_shape->get_next_node(mouse_world);
				if (m_selected_node == nullptr) {
                    printf("shape push_back\n");
                    shapes.push_back(temp_shape);
                    changed = true;
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

void View2D::world_to_scr(Vector2f world, int& scrx, int& scry)
{
	scrx = static_cast<int>((world.x - world_offset.x) * world_scale);
	scry = static_cast<int>((world.y - world_offset.y) * world_scale);
}

void View2D::scr_to_world(int scrx, int scry, Vector2f& world)
{
	world.x = static_cast<float>(scrx) / world_scale + world_offset.x;
	world.y = static_cast<float>(scry) / world_scale + world_offset.y;
}

void View2D::draw_create_shape()
{
    
}

void View2D::pan(int scrx, int scry)
{
    float scrxf = static_cast<float>(scrx);
    float scryf = static_cast<float>(scry);
    world_offset.x -= scrxf / world_scale;
    world_offset.y -= scryf / world_scale;
    redraw();
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
		else if (world_offset.x > v2d_w) world_offset.x = static_cast<float>(v2d_w);
		if (world_offset.y < -h()) world_offset.y = static_cast<float>(-h());
		else if (world_offset.y > v2d_h) world_offset.y = static_cast<float>(v2d_h);
	}
    redraw();
}

void View2D::zoom(float scale_factor_percent, int focusx, int focusy)
{
    Vector2f bf_center_axis;
	scr_to_world(focusx, focusy, bf_center_axis);

	world_scale *= scale_factor_percent;
    
    if (world_scale < min_zoom) world_scale = min_zoom;
    else if (world_scale > max_zoom) world_scale = max_zoom;

	Vector2f af_center_axis;
	scr_to_world(focusx, focusy, af_center_axis);

	world_offset.x += (bf_center_axis.x - af_center_axis.x);
	world_offset.y += (bf_center_axis.y - af_center_axis.y);

	redraw();
}

void View2D::change_cursor(Fl_Cursor c)
{
    if (c == current_cursor) return;
    last_cursor = current_cursor;
    current_cursor = c;
    window()->cursor(c);
}


void View2D::draw_axes(int centerx, int centery, int w, int h, int line_width)
{
    // Draw axes
	fl_color(FL_RED);
	fl_line_style(FL_SOLID, line_width);
	fl_line(centerx, centery, w, centery);
	fl_line_style(FL_DOT, line_width);
	fl_line(0, centery, centerx, centery);

	fl_color(FL_GREEN);
	fl_line_style(FL_SOLID, line_width);
	fl_line(centerx, centery, centerx, h);
	fl_color(FL_GREEN);
	fl_line_style(FL_DOT, line_width);
	fl_line(centerx, 0, centerx, centery);
}

void View2D::get_cursor_v2d_position(int &cx, int &cy)
{
    cx = Fl::event_x() - x();
    cy = Fl::event_y() - y();
}

Vector2f View2D::get_snap_grid(Vector2f vec2d_world)
{
    Vector2f result;
    result.x = floorf((vec2d_world.x + 0.5f) * snap_grid_interval);
    result.y = floorf((vec2d_world.y + 0.5f) * snap_grid_interval);
    return result;
}

void View2D::draw_grid(int point_sz)
{
    // TODO(daniel): performance really poor
    Vector2f v2d_origin_world;
    scr_to_world(v2d_x, v2d_y, v2d_origin_world);
    v2d_origin_world.x -= snap_grid_interval;
    v2d_origin_world.y -= snap_grid_interval;

    Vector2f v2d_size_world;
    scr_to_world(v2d_w, v2d_h, v2d_size_world);
    v2d_size_world.x += snap_grid_interval;
    v2d_size_world.y += snap_grid_interval;

    v2d_origin_world = get_snap_grid(v2d_origin_world);

    int scrx, scry;
    for (float xf = v2d_origin_world.x; xf < v2d_size_world.x; xf += snap_grid_interval) {
        for (float yf = v2d_origin_world.y; yf < v2d_size_world.y; yf += snap_grid_interval) {
            world_to_scr(Vector2f{ xf, yf }, scrx, scry);
            fl_color(FL_WHITE);
            fl_point(scrx, scry);
            //fl_circle(scrx, scry, 2);
        }
    }
    //printf("%d - %f, %f - %f, %f\n", point_counter, v2d_origin_world.x, v2d_size_world.x, v2d_origin_world.y, v2d_size_world.y);
}
