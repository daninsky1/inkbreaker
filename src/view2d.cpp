#include "view2d.h"

constexpr int DRAG_THRESHOLD = 5;

View2D::View2D(int x, int y, int w, int h, std::vector<Shape*> &p_shapes) :
    Fl_Box{ x, y, w, h },
    mouse_current_world{ mouse_snap_world },
    shapes { p_shapes }
{
    state.mode = Mode::draw;
    state.select = Select::move;
    state.draw = Draw::line;

    scr_buf = fl_create_offscreen(w, h);
    fl_offscr_scale = Fl_Graphics_Driver::default_driver().scale();

    v2d_x = x; v2d_y = y;
    v2d_w = w; v2d_h = h;
} // View2D

void View2D::DEBUG_display_v2d_info()
{
    // SCREEN DEBUG MESSAGES
    // NOTE(daniel): Update mouse view2d coordinates, and gets(not update) the
    // snap cursor view2d coordinates
    // TODO(daniel): The handles call entire draws to update the coordinate
    // messages, try to use damage technique no avoid entire redraws:
    // http://seriss.com/people/erco/fltk/#DrawCoords

    fl_color(FL_WHITE);
    int font = FL_COURIER;
    int font_sz = 14;
    fl_font(font, 15);

    int pad = 10;

    const int msg_sz = 128;
    char scr_dmsg[msg_sz];
    sprintf(scr_dmsg, "View2D: (%d x %d)", v2d_w, v2d_h);
    fl_draw(scr_dmsg, v2d_x + pad, fl_height(font, font_sz));
    sprintf(scr_dmsg, "World offset: (%.3f, %.3f)", world_offset.x, world_offset.y);
    fl_draw(scr_dmsg, v2d_x + pad, fl_height(font, font_sz) * 2);
    sprintf(scr_dmsg, "World scale: %.3f", world_scale);
    fl_draw(scr_dmsg, v2d_x + pad, fl_height(font, font_sz) * 3);

    sprintf(scr_dmsg, "mouse world: (%.3f, %.3f)", mouse_world.x, mouse_world.y);
    fl_draw(scr_dmsg, v2d_x + pad, h() - pad - fl_height(font, font_sz)*3);
    sprintf(scr_dmsg, "mouse world snap: (%.3f, %.3f)", mouse_snap_world.x, mouse_snap_world.y);
    fl_draw(scr_dmsg, v2d_x + pad, v2d_h - pad - fl_height(font, font_sz)*2);
    sprintf(scr_dmsg, "mouse view2d: (%d, %d)", mouse_v2d.x, mouse_v2d.y);
    fl_draw(scr_dmsg, v2d_x + pad, v2d_h - pad - fl_height(font, font_sz));
    sprintf(scr_dmsg, "mouse view2d snap: (%d, %d)", snap_cursor_v2d.x, snap_cursor_v2d.y);
    fl_draw(scr_dmsg, v2d_x + pad, v2d_h - pad);
}

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
    for (size_t i = 0; i < shapes.size(); ++i) {
        shapes[i]->draw_shape();
    }

    if (temp_shape) {
        temp_shape->draw_shape();
        //temp_shape->draw_nodes();
    }

    for (size_t i = 0; i < bshapes.size(); ++i) {
        bshapes[i]->draw_shape();
    }

    if (bezier_temp_shape) {
        bezier_temp_shape->draw_shape();
        if (active_bhandle) {
            // Draw active handle
            fl_color(FL_WHITE);
            fl_line_style(FL_SOLID, 2);
            int tx, ty, px, py, hx, hy;
            world_to_scr(active_bhandle->tail, tx, ty);
            world_to_scr(active_bhandle->point, px, py);
            world_to_scr(active_bhandle->head, hx, hy);
            fl_line(tx, ty, px, py);
            fl_line(px, py, hx, hy);
            fl_color(FL_RED);
            fl_circle(hx, hy, 3);
            fl_circle(tx, ty, 3);
        }
    }

    //if (is_selecting) {
    //    select_shape_bbox->draw_shape();
    //}

    //if (app_state->active_selection) {
    //    app_state->active_selection->draw_bbox();
    //}

    // DRAW SNAP CURSOR
    Vec2f scr_origin_to_world;
    scr_to_world(0, 0, scr_origin_to_world);
    Vec2f scr_size_to_world;
    scr_to_world(v2d_w, v2d_h, scr_size_to_world);

    if (world_scale >= 15.0f) {
        fl_line_style(FL_SOLID, 1);
        fl_color(FL_YELLOW);
        fl_circle(snap_cursor_v2d.x, snap_cursor_v2d.y, 4);
    }

    DEBUG_display_v2d_info();

    fl_end_offscreen();
    fl_copy_offscreen(v2d_x, v2d_y, v2d_w, v2d_h, scr_buf, 0, 0);
}


int View2D::handle(int evt)
{
    if (Fl_Box::handle(evt)) return 1;

    // Focus keyboard events to this widget
    switch (evt) {
    case FL_FOCUS: case FL_UNFOCUS:
        return 1;
        break;
    default:
        break;
    }

    if (state.mode == Mode::draw) {
        if (handle_edit_mode(evt)) return 1;
    }
    
    

    int handled = 0;

    int key_code = 0;
    if (evt == FL_KEYBOARD) {
        constexpr int bfsz = 100;
        char buffer[bfsz];
        const char* keyname = buffer;
        key_code = Fl::event_key();

        if (key_code < 128) { // ASCII
            sprintf(buffer,"'%c'", key_code);
            switch (key_code) {
            case 'r': {
                std::cout << v2d_w << ", " << v2d_h << " " << w() << ", " << h() << std::endl;
                handled = 1;
                redraw();
            } break;
            case 'z':
                std::cout << "ZOOM_MODE\n";
                m_lm_state = Mode::zoom;
                m_md_scr_msg = "zoom";
                handled = 1;
                redraw();
                break;
            case 'h': case ' ':
                std::cout << "DRAG_MODE\n";
                m_lm_state = Mode::pan;
                m_md_scr_msg = "pan";
                handled = 1;
                redraw();
                break;
            case 'l':
                std::cout << "DRAW_MODE\n";
                m_lm_state = Mode::draw;
                m_md_scr_msg = "draw";
                handled = 1;
                redraw();
                break;
            case '0':
                std::cout << "ZERO_MODE\n";
                m_lm_state = Mode::select;
                m_md_scr_msg = "default";
                handled = 1;
                redraw();
                break;
            }
        }
    }

    handle_pan_tilt_zoom(evt);

    // NOTE(daniel): Draw shapes by dragging
    if (state.mode == Mode::select) {
        switch (evt) {
        case FL_PUSH: {
            get_cursor_v2d_position(mouse_v2d.x, mouse_v2d.y);
            if (Fl::event_button() == FL_LEFT_MOUSE) {
                if (!select_shape_bbox) {
                    select_shape_bbox = new BBox();
                    // first node at location of left click
                    select_shape_bbox->get_next_node(mouse_world);
                    select_shape_bbox->get_next_node(mouse_world);
                }
                else {
                    select_shape_bbox->nodes[0].pos = mouse_world;
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
            handled = 1;
        } break;
        case FL_DRAG: {
            get_cursor_v2d_position(mouse_v2d.x, mouse_v2d.y);
            scr_to_world(mouse_v2d.x, mouse_v2d.y, mouse_world);

            if (is_selecting) {
                // second node
                select_shape_bbox->nodes[1].pos = mouse_world;
            }
            else if (is_moving) {
                translate((mouse_world.x - drag_start_world.x), (mouse_world.y - drag_start_world.y),
                    app_state->active_selection);
                drag_start_world.x = mouse_world.x;
                drag_start_world.y = mouse_world.y;
                app_state->active_selection->update_bbox();
                //printf("drag world distance: %f, %f\n", (drag_start_world.x - mouse_world.x), (drag_start_world.y - mouse_world.y));
                //printf("%s position: %f, %f - %f, %f\n", active_selection->type().c_str(), active_selection->nodes[0].pos.x, active_selection->nodes[0].pos.y, active_selection->nodes[1].pos.x, active_selection->nodes[1].pos.y);
            }
            redraw();
            handled = 1;
        } break;
        case FL_RELEASE: {
            // TODO(daniel): Finish and store selection
            get_cursor_v2d_position(mouse_v2d.x, mouse_v2d.y);
            scr_to_world(mouse_v2d.x, mouse_v2d.y, mouse_world);
            mouse_snap_world = get_snap_grid(mouse_world);
            world_to_scr(mouse_snap_world, snap_cursor_v2d.x, snap_cursor_v2d.y);

            if (is_selecting) {
                if (select_shape_bbox->scrw > DRAG_THRESHOLD || select_shape_bbox->scrh > DRAG_THRESHOLD) {
                    // Do box selection
                }
                else {
                    // Do click seletion
                    for (std::vector<Shape*>::reverse_iterator it = shapes.rbegin(); it < shapes.rend(); ++it) {
                        if ((*it)->is_inside_bbox(mouse_world)) {
                            *it = app_state->active_selection;
                            app_state->active_selection = *it;
                            printf("%s position: %f, %f\n", app_state->active_selection->type().c_str(), app_state->active_selection->nodes[0].pos.x, app_state->active_selection->nodes[0].pos.y);
                            break;
                        }
                        app_state->active_selection = nullptr;
                    }
                }
            }
            else if (is_moving) {
                printf("stop moving\n");
                is_moving = false;
            }

            is_selecting = false;
            redraw();
            
            handled = 1;
        } break;
        }
    }

    //else if (state.mode == Mode::draw) {
    //    // NOTE(daniel): This part only works for shapes that has two nodes max
    //	switch (evt) {
    //	case FL_PUSH:
    //		if (Fl::event_button() == FL_LEFT_MOUSE) {
    //		}
    //		handled = 1;
    //		break;
    //	case FL_DRAG: {
    //        get_cursor_v2d_position(mouse_v2d.x, mouse_v2d.y);
    //		scr_to_world(Fl::event_x() - x(), Fl::event_y() - y(), mouse_world);
    //		// second node
    //		if (active_point != nullptr) {
    //			active_point->pos = mouse_world;
    //		}
    //		redraw();
    //		
    //		handled = 1;
    //    } break;
    //	case FL_RELEASE: {
    //        
    //        
    //		if (is_drawing) {
    //			active_point = temp_shape->get_next_node(mouse_world);
    //            is_drawing = false;
    //		}
    //		handled = 1;
    //    } break;
    //	}
    //}

    return handled;
} // View2D::handle

int View2D::handle_pan_tilt_zoom(int evt)
{
    /* Handles pan-tilt-zoom of the view2D */
    int handled = 0;
    
    switch (evt) {
    case FL_MOVE: {
        get_cursor_v2d_position(mouse_v2d.x, mouse_v2d.y);
        scr_to_world(mouse_v2d.x, mouse_v2d.y, mouse_world);
        mouse_snap_world = get_snap_grid(mouse_world);
        world_to_scr(mouse_snap_world, snap_cursor_v2d.x, snap_cursor_v2d.y);
        redraw();
        handled = 1;
    } break;
    case FL_PUSH: {
        scr_to_world(mouse_v2d.x, mouse_v2d.y, mouse_world);
        mouse_snap_world = get_snap_grid(mouse_world);

        if (Fl::event_button() == FL_MIDDLE_MOUSE) {
            change_cursor(FL_CURSOR_MOVE);
        }
        else if (state.mode == Mode::select) {
        }

        scr_to_world(mouse_v2d.x, mouse_v2d.y, drag_start_world);
        drag_start_world = get_snap_grid(drag_start_world);
        drag_start_scr_x = Fl::event_x_root();
        drag_start_scr_y = Fl::event_y_root();

        handled = 1;
    } break;
    case FL_DRAG: {
        get_cursor_v2d_position(mouse_v2d.x, mouse_v2d.y);
        scr_to_world(mouse_v2d.x, mouse_v2d.y, mouse_world);
        mouse_snap_world = get_snap_grid(mouse_world);
        world_to_scr(mouse_snap_world, snap_cursor_v2d.x, snap_cursor_v2d.y);
        is_dragging = true;

        int drag_update_scrx = Fl::event_x_root();
        int drag_update_scry = Fl::event_y_root();

        if ((Fl::event_button() == FL_MIDDLE_MOUSE) || (state.mode == Mode::pan && Fl::event_button() == FL_LEFT_MOUSE)) {
            pan(drag_update_scrx - drag_start_scr_x, drag_update_scry - drag_start_scr_y);
        }
        else if (state.mode == Mode::zoom && Fl::event_button() == FL_LEFT_MOUSE) {
            float drag_dist_pix = static_cast<float>(drag_update_scrx - drag_start_scr_x);
            float scale_factor_per_pixel = drag_dist_pix * zooming_sens;
            float scale_factor_percent = scale_factor_per_pixel + 1.0f;

            // NOTE(daniel): Maybe use drag start to center zoom
            zoom(scale_factor_percent, v2d_w / 2, v2d_h / 2);

            world_to_scr(drag_start_world, snap_cursor_v2d.x, snap_cursor_v2d.y);

            //Printf("%f * %f = %f\n", drag_dist_pix, zooming_sens, scale_factor_percent);
        }

        drag_start_scr_x = drag_update_scrx;
        drag_start_scr_y = drag_update_scry;
        redraw();
        handled = 1;
    } break;
    case FL_RELEASE: {
        scr_to_world(mouse_v2d.x, mouse_v2d.y, mouse_world);
        mouse_snap_world = get_snap_grid(mouse_world);

        if (is_dragging) {
            is_dragging = false;
            change_cursor(FL_CURSOR_DEFAULT);
        }
        redraw();
        handled = 1;
    } break;
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
        handled = 1;
    } break;
    default:
        break;
    }
    
    return handled;
}

int View2D::handle_edit_mode(int evt)
{
    int handled = 0;
    if (state.mode != Mode::draw) {
        // Nothing to do
        is_drawing = false;
        return handled;
    }

    if (evt == FL_KEYBOARD) {
        int key_code = Fl::event_key();
        switch (key_code) {
        case FL_Enter: {
            if (is_drawing) {
                if (temp_shape && (temp_shape->type() == "polygon")) {
                    if (temp_shape->nodes.size() <= 2) {
                        is_drawing = false;
                        active_point = nullptr;
                        delete temp_shape;
                        temp_shape = nullptr;
                    }
                    else {
                        is_drawing = false;
                        temp_shape->nodes.pop_back();
                        shapes.push_back(temp_shape);
                        active_point = nullptr;
                        temp_shape = nullptr;
                    }
                }
                else if (temp_shape && (temp_shape->type() == "line")
                        && (temp_shape->type() == "rect")
                        && (temp_shape->type() == "circle")) {
                    shapes.push_back(temp_shape);
                    is_drawing = false;
                    active_point = nullptr;
                    temp_shape = nullptr;
                }

                if (bezier_temp_shape) {
                    // TODO(daniel): Implement this
                    if (bezier_temp_shape->bhandles.size() < 2) {
                        is_drawing = false;
                        active_bhandle = nullptr;
                        delete bezier_temp_shape;
                        bezier_temp_shape = nullptr;
                    }
                    else {
                        is_drawing = false;
                        bezier_temp_shape->bhandles.pop_back();
                        bshapes.push_back(bezier_temp_shape);
                        active_bhandle = nullptr;
                        bezier_temp_shape = nullptr;
                    }
                }
                redraw();
            }
            handled = 1;
        } break;
        case FL_Escape: {
            if (is_drawing) {
                is_drawing = false;
                active_point = nullptr;
                delete temp_shape;
                temp_shape = nullptr;
                redraw();
            }
            if (is_selecting) {

            }
            handled = 1;
        } break;
        case FL_BackSpace: {
            if (is_drawing) {
                if (temp_shape) {
                    if (temp_shape->nodes.size() <= 2) {
                        is_drawing = false;
                        active_point = nullptr;
                        delete temp_shape;
                        temp_shape = nullptr;
                    }
                    else {
                        temp_shape->nodes.pop_back();
                        active_point = &temp_shape->nodes[temp_shape->nodes.size() - 1];
                        printf("nodes size %lu\n", temp_shape->nodes.size());
                    }
                }
                else if (bezier_temp_shape) {
                    if (bezier_temp_shape->bhandles.size() < 2) {
                        is_drawing = false;
                        active_bhandle = nullptr;
                        delete bezier_temp_shape;
                        bezier_temp_shape = nullptr;
                    }
                    else {
                        bezier_temp_shape->bhandles.pop_back();
                    }
                }

                get_cursor_v2d_position(mouse_v2d.x, mouse_v2d.y);
                scr_to_world(mouse_v2d.x, mouse_v2d.y, mouse_world);
                mouse_snap_world = get_snap_grid(mouse_world);
                world_to_scr(mouse_snap_world, snap_cursor_v2d.x, snap_cursor_v2d.y);

                redraw();
            }
            handled = 1;
        } break;
        }
    }

    // TODO(daniel): Review redraw() calls and handled state to be called or
    // changed at right moment.
    switch (evt) {
    case FL_MOVE: {
        if (is_drawing) {
            get_cursor_v2d_position(mouse_v2d.x, mouse_v2d.y);
            scr_to_world(mouse_v2d.x, mouse_v2d.y, mouse_world);
            mouse_snap_world = get_snap_grid(mouse_world);
            world_to_scr(mouse_snap_world, snap_cursor_v2d.x, snap_cursor_v2d.y);
            if (active_bhandle) {
                active_bhandle->point = mouse_current_world;
                active_bhandle->head = mouse_current_world;
                active_bhandle->tail = mouse_current_world;
            }
            if (active_point) {
                active_point->pos = mouse_current_world;
            }
            redraw();
            handled = 1;
        }
    } break;
    case FL_PUSH: {
        get_cursor_v2d_position(mouse_v2d.x, mouse_v2d.y);
        scr_to_world(mouse_v2d.x, mouse_v2d.y, mouse_world);
        mouse_snap_world = get_snap_grid(mouse_world);
        world_to_scr(mouse_snap_world, snap_cursor_v2d.x, snap_cursor_v2d.y);

        if (Fl::event_button() == FL_LEFT_MOUSE) {
            // NOTE(daniel): Bezier curve starts to draw at left mouse draw,
            // unlike the other shapes, because has the drag feature to move
            // the bezier handles
            if (state.draw == Draw::bezier) {
                if (!is_drawing) {
                    bezier_temp_shape = new Bezier();
                    bezier_temp_shape->shape_info = shape_info;

                    /* Set the first bezier handle and get the next bezier handle */
                    active_bhandle = bezier_temp_shape->get_next_handle(
                        BezierHandle{ mouse_current_world, mouse_current_world, mouse_current_world }
                    );

                    is_drawing = true;
                }
            }
        }

        // Review if this is necessary
        drag_start_scr_x = Fl::event_x_root();
        drag_start_scr_y = Fl::event_y_root();

        //redraw();
        handled = 1;
    } break;
    case FL_DRAG: {
        get_cursor_v2d_position(mouse_v2d.x, mouse_v2d.y);
        scr_to_world(mouse_v2d.x, mouse_v2d.y, mouse_world);
        mouse_snap_world = get_snap_grid(mouse_world);
        world_to_scr(mouse_snap_world, snap_cursor_v2d.x, snap_cursor_v2d.y);

        // NOTE(daniel): Find a way to track dragging state(is_dragging) that
        // is tied to this function/state
        int drag_update_scrx = Fl::event_x_root();
        int drag_update_scry = Fl::event_y_root();

        if (is_drawing && (Fl::event_button() == FL_LEFT_MOUSE)) {
            // Update active head bhandle and tail handle
            if (active_bhandle) {
                active_bhandle->head = mouse_current_world;
                // Invert mouse coordinates
                active_bhandle->tail.x = active_bhandle->point.x + (active_bhandle->point.x - active_bhandle->head.x);
                active_bhandle->tail.y = active_bhandle->point.y + (active_bhandle->point.y - active_bhandle->head.y);
                //printf("head: p0: %f, %f | h0: %f, %f | tail: h1: %f, %f | p1: %f, %f\n");
            }
            drag_start_scr_x = drag_update_scrx;
            drag_start_scr_y = drag_update_scry;

            redraw();
            handled = 1;
        }
    } break;
    case FL_RELEASE: {
        scr_to_world(mouse_v2d.x, mouse_v2d.y, mouse_world);
        mouse_snap_world = get_snap_grid(mouse_world);

        if (Fl::event_button() == FL_LEFT_MOUSE) {
            // first node at location of left click
            if (!is_drawing) {
                if (state.draw == Draw::line) {
                    temp_shape = new Line();
                }
                else if (state.draw == Draw::rect) {
                    temp_shape = new Rect();
                }
                else if (state.draw == Draw::circle) {
                    temp_shape = new Circle();
                }
                else if (state.draw == Draw::poly) {
                    temp_shape = new Poly();
                }

                // NOTE(daniel): If this assertion fail means that the draw
                // states was set incorrecly, which is rare because we are
                // using enumerators
                assert(temp_shape);

                if (temp_shape) {
                    temp_shape->get_next_node(mouse_current_world);
                    active_point = temp_shape->get_next_node(mouse_current_world);
                    is_drawing = true;
                }
                else {
                    fprintf(stderr, "Error creating shape: temp_shape.");
                }
                temp_shape->shape_info = shape_info;
            }
            else {
                printf("mouse release: %f, %f - %f, %f\n", mouse_world.x, mouse_world.y, mouse_snap_world.x, mouse_snap_world.y);

                if (bezier_temp_shape) {
                    active_bhandle = bezier_temp_shape->get_next_handle(*active_bhandle);
                }
                else {
                    active_point = temp_shape->get_next_node(mouse_current_world);

                    printf("nodes size %lu\n", temp_shape->nodes.size());
                    if (!active_point) {
                        shapes.push_back(temp_shape);
                        printf("Shape stored: %s\n", temp_shape->type().c_str());
                        printf("%f, %f - %f, %f\n", temp_shape->nodes[0].pos.x, temp_shape->nodes[0].pos.y, temp_shape->nodes[1].pos.x, temp_shape->nodes[1].pos.y);
                        is_drawing = false;
                        changed = true;
                        temp_shape = nullptr;
                        active_point = nullptr;
                    }
                }
            }
        }
        is_dragging = false;
        redraw();
        handled = 1;
    } break;
    default: {
    }

    }

    return handled;
}

void View2D::world_to_scr(Vec2f world, int& scrx, int& scry)
{
	scrx = static_cast<int>((world.x - world_offset.x) * world_scale);
	scry = static_cast<int>((world.y - world_offset.y) * world_scale);
}

void View2D::scr_to_world(int scrx, int scry, Vec2f& world)
{
	world.x = static_cast<float>(scrx) / world_scale + world_offset.x;
	world.y = static_cast<float>(scry) / world_scale + world_offset.y;
}

void View2D::pan(int scrx, int scry)
{
    float scrxf = static_cast<float>(scrx);
    float scryf = static_cast<float>(scry);
    world_offset.x -= scrxf / world_scale;
    world_offset.y -= scryf / world_scale;
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
    Vec2f bf_center_axis;
	scr_to_world(focusx, focusy, bf_center_axis);

	world_scale *= scale_factor_percent;
    
    if (world_scale < min_zoom) world_scale = min_zoom;
    else if (world_scale > max_zoom) world_scale = max_zoom;

	Vec2f af_center_axis;
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

Vec2f View2D::get_snap_grid(Vec2f vec2d_world)
{
    Vec2f result;
    result.x = floorf((vec2d_world.x + 0.5f) * snap_grid_interval);
    result.y = floorf((vec2d_world.y + 0.5f) * snap_grid_interval);
    return result;
}

void View2D::draw_grid(int point_sz)
{
    // TODO(daniel): performance really poor
    Vec2f v2d_origin_world;
    scr_to_world(v2d_x, v2d_y, v2d_origin_world);
    v2d_origin_world.x -= snap_grid_interval;
    v2d_origin_world.y -= snap_grid_interval;

    Vec2f v2d_size_world;
    scr_to_world(v2d_w, v2d_h, v2d_size_world);
    v2d_size_world.x += snap_grid_interval;
    v2d_size_world.y += snap_grid_interval;

    v2d_origin_world = get_snap_grid(v2d_origin_world);

    int scrx, scry;
    for (float xf = v2d_origin_world.x; xf < v2d_size_world.x; xf += snap_grid_interval) {
        for (float yf = v2d_origin_world.y; yf < v2d_size_world.y; yf += snap_grid_interval) {
            world_to_scr(Vec2f{ xf, yf }, scrx, scry);
            fl_color(FL_WHITE);
            fl_point(scrx, scry);
            //fl_circle(scrx, scry, 2);
        }
    }
    //printf("%d - %f, %f - %f, %f\n", point_counter, v2d_origin_world.x, v2d_size_world.x, v2d_origin_world.y, v2d_size_world.y);
}

void set_mouse_current_world(Vec2f &mouse)
{

}
