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

	// TODO: IMPLEMENT GRID
	// // GRID
	// // TODO: IMPLEMENT GRID OBJECT
	// /*
	// * 
	// fl_line_style(FL_SOLID, 2.0f*world_scale);
	// // horizontal lines
	// float gridx = 100.0f, gridy = 100.0f;
	// for (double y = 0.0; y <= gridy; y+=gridy/10.0) {
	// 	Vector2f start{ 0.0, y };
	// 	Vector2f end{ gridx, y };

	// 	// world to screen
	// 	int start_sspx, start_sspy, end_sspx, end_sspy;
	// 	world_to_scr(start, start_sspx, start_sspy);
	// 	world_to_scr(end, end_sspx, end_sspy);

	// 	fl_line(start_sspx, start_sspy, end_sspx, end_sspy);
	// }

	// //// vertical lines
	// for (double x = 0.0; x <= gridx; x+=gridx/10.0) {
	// 	Vector2f start{ x, 0.0 };
	// 	Vector2f end{ x, gridy };
	// 	int startx = x, starty = 0.0f;
	// 	int endx = x, endy = gridy;

	// 	// world to screen
	// 	int start_sspx, start_sspy, end_sspx, end_sspy;
	// 	world_to_scr(start, start_sspx, start_sspy);
	// 	world_to_scr(end, end_sspx, end_sspy);

	// 	fl_line(start_sspx, start_sspy, end_sspx, end_sspy);
	// }
	// */
    
    // DRAW SNAP CURSOR AND GRID
    int snap_cx;
    int snap_cy;
    world_to_scr(mouse_snap_world, snap_cx, snap_cy);

    Vector2f scr_origin_to_world;
    scr_to_world(0, 0, scr_origin_to_world);
    Vector2f scr_size_to_world;
    scr_to_world(v2d_w, v2d_h, scr_size_to_world);

    if (world_scale >= 15.0f) {
        // Draw snap cursor
        fl_line_style(FL_SOLID, 1);
        fl_color(FL_YELLOW);
        fl_circle(snap_cx, snap_cy, 1);
        // Draw grid
        draw_grid(5);
    }

    fl_end_offscreen();
    fl_copy_offscreen(v2d_x, v2d_y, v2d_w, v2d_h, scr_buf, 0, 0);


	// SCREEN DEBUG INFO
	fl_color(FL_WHITE);
	int font = FL_COURIER;
	int font_sz = 14;
	fl_font(font, 15);

	int pad = 10;
	// screen mouse position
	std::stringstream ss_log;
	ss_log << std::fixed;
	ss_log << "View2D Size: " << '(' << v2d_w << " x " << v2d_h << ')';
	fl_draw(ss_log.str().c_str(), v2d_x + pad, v2d_y + fl_height(font, font_sz));
	ss_log.str(std::string{""});
	ss_log.str("");
	ss_log << "World Offset: " << '(' << world_offset.x << " ," << world_offset.y << ')';
	fl_draw(ss_log.str().c_str(), v2d_x + pad, v2d_y + fl_height(font, font_sz) * 2);
	ss_log.str("");
	ss_log << "Scale: " << world_scale;
	fl_draw(ss_log.str().c_str(), v2d_x + pad, v2d_y + fl_height(font, font_sz) * 3);

	// mode
    //fl_draw(m_md_scr_msg.c_str(), v2d_x+pad, h() - pad - fl_height(font, font_sz)*4);
	std::string mouse_coor;
	mouse_coor.append("mouse screen (");
	mouse_coor.append(std::to_string(mouse_v2d.x));
	mouse_coor.append(" ,");
	mouse_coor.append(std::to_string(mouse_v2d.y));
	mouse_coor.append(")");
	fl_draw(mouse_coor.c_str(), v2d_x + pad, h() - pad - fl_height(font, font_sz)*3);
	// world mouse position
	mouse_coor = "";
	mouse_coor.append("mouse world (");
	mouse_coor.append(std::to_string(mouse_world.x));
	mouse_coor.append(" ,");
	mouse_coor.append(std::to_string(mouse_world.y));
	mouse_coor.append(" )");
	fl_draw(mouse_coor.c_str(), v2d_x + pad, v2d_h - pad - fl_height(font, font_sz)*2);
	ss_log.str("");
	ss_log << "mouse world snap: (" << mouse_snap_world.x << ", " << mouse_snap_world.y << ')';
	fl_draw(ss_log.str().c_str(), v2d_x + pad, v2d_h - pad - fl_height(font, font_sz));
	ss_log.str("");
	ss_log << "mouse screen snap: (" << snap_cx << ", " << snap_cy << ')';
	fl_draw(ss_log.str().c_str(), v2d_x + pad, v2d_h - pad);
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
	case FL_MOVE:
        get_cursor_v2d_position(mouse_v2d.x, mouse_v2d.y);
		scr_to_world(mouse_v2d.x, mouse_v2d.y, mouse_world);

        //if (world_scale >= 1.0f) {
        //    mouse_snap_world = get_snap_grid(mouse_world);
        //}

		redraw();
		ret = 1;
		break;
	case FL_PUSH:
		if (Fl::event_button() == FL_MIDDLE_MOUSE) {
            change_cursor(FL_CURSOR_MOVE);
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

				zoom(v2d_w / 2, v2d_h / 2, scale_factor_percent);

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
        change_cursor(FL_CURSOR_DEFAULT);
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

	if (state.mode == Mode::select) {
		switch (evt) {
		case FL_PUSH: {
			mouse_v2d.x = Fl::event_x() - x();
			mouse_v2d.y = Fl::event_y() - y();
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
                    drag_world_start_pos = mouse_world;
                    is_selecting = false;
                    is_moving = true;
                    changed = true;
                }
            }
			ret = 1;
        } break;
		case FL_DRAG: {
			mouse_v2d.x = Fl::event_x() - x();
			mouse_v2d.y = Fl::event_y() - y();
			scr_to_world(Fl::event_x() - x(), Fl::event_y() - y(), mouse_world);

            if (is_selecting) {
                // second node
                select_box->nodes[1].pos = mouse_world;
            }
            else if (is_moving) {
                for (int i = 0; i < app_state->active_selection->nodes.size(); ++i) {
                    app_state->active_selection->nodes[i].pos.x += (mouse_world.x - drag_world_start_pos.x);
                    app_state->active_selection->nodes[i].pos.y += (mouse_world.y - drag_world_start_pos.y);
                }
                drag_world_start_pos.x = mouse_world.x;
                drag_world_start_pos.y = mouse_world.y;
                app_state->active_selection->update_bbox();
                //printf("drag world distance: %f, %f\n", (drag_world_start_pos.x - mouse_world.x), (drag_world_start_pos.y - mouse_world.y));
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

void View2D::zoom(int focusx, int focusy, float scale_factor_percent)
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
    Vector2f v2d_origin_world;
    scr_to_world(v2d_x, v2d_y, v2d_origin_world);
    Vector2f v2d_size_world;
    scr_to_world(v2d_w, v2d_h, v2d_size_world);
    
    while (v2d_origin_world.x < v2d_size_world.x) {
        
    }
}
