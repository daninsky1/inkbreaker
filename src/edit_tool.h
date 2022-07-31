/* This draw and edit by mouse input for now */
#pragma once

#include <FL/Enumerations.H>

#include "enumerators.h"
#include "coordinates.h"
#include "objects/polygon.h"


struct EditHandle {
    bool is_active = false;
};

class EditTool {
    Draw draw_tool_type = Draw::polygon;
    EditHandle *active_handle;
    Vec2f margin{ 10.0f, 10.0f };
};

class PolygonTool {
public:
    PolygonTool(Polygon *polygon) :
        active_polygon{ polygon } { is_active = true; }
    bool is_active() { return is_active; };
    bool is_active() { return is_active; };
    int create_polygon_handle(int evt);
    int edit_polygon(int evt);
    void draw_guides();
    void draw_nodes();
    void draw_outline();
private:
    Polygon *active_polygon;
    bool is_active;
};

Polygon *create_polygon()
{
    
}

int PolygonTool::create_polygon_handle(int evt)
{
    int handled = 0;

    // TODO(daniel): Review redraw() calls and handled state to be called or
    // changed at right moment.
    switch (evt) {
    case FL_MOVE: {
        if (is_active) {
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
            // redraw(); // Redraw will always be called
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
                if (!is_active) {
                    bezier_temp_shape = new Bezier();
                    bezier_temp_shape->shape_info = shape_info;

                    /* Set the first bezier handle and get the next bezier handle */
                    active_bhandle = bezier_temp_shape->get_next_handle(
                        BezierHandle{ mouse_current_world, mouse_current_world, mouse_current_world }
                    );

                    is_active = true;
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

        if (is_active && (Fl::event_button() == FL_LEFT_MOUSE)) {
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
            if (!is_active) {
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
                    is_active = true;
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

                    printf("nodes size %llu\n", temp_shape->nodes.size());
                    if (!active_point) {
                        shapes.push_back(temp_shape);
                        printf("Shape stored: %s\n", temp_shape->type().c_str());
                        printf("%f, %f - %f, %f\n", temp_shape->nodes[0].pos.x, temp_shape->nodes[0].pos.y, temp_shape->nodes[1].pos.x, temp_shape->nodes[1].pos.y);
                        is_active = false;
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
    
    if (evt == FL_KEYBOARD) {
        int key_code = Fl::event_key();
        switch (key_code) {
        case FL_Enter: {
            if (is_active) {
                if (temp_shape && (temp_shape->type() == "polygon")) {
                    if (temp_shape->nodes.size() <= 2) {
                        is_active = false;
                        active_point = nullptr;
                        delete temp_shape;
                        temp_shape = nullptr;
                    }
                    else {
                        is_active = false;
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
                    is_active = false;
                    active_point = nullptr;
                    temp_shape = nullptr;
                }

                if (bezier_temp_shape) {
                    // TODO(daniel): Implement this
                    if (bezier_temp_shape->bhandles.size() < 2) {
                        is_active = false;
                        active_bhandle = nullptr;
                        delete bezier_temp_shape;
                        bezier_temp_shape = nullptr;
                    }
                    else {
                        is_active = false;
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
            if (is_active) {
                is_active = false;
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
            if (is_active) {
                if (temp_shape) {
                    if (temp_shape->nodes.size() <= 2) {
                        is_active = false;
                        active_point = nullptr;
                        delete temp_shape;
                        temp_shape = nullptr;
                    }
                    else {
                        temp_shape->nodes.pop_back();
                        active_point = &temp_shape->nodes[temp_shape->nodes.size() - 1];
                        printf("nodes size %llu\n", temp_shape->nodes.size());
                    }
                }
                else if (bezier_temp_shape) {
                    if (bezier_temp_shape->bhandles.size() < 2) {
                        is_active = false;
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
    
    return handled;
}

int edit_polygon(int evt)
{
    
}

class CreateBezierHandle : public EditHandle {
    int create_bezier(int evt ) { return 0; }
    int edit_bezier(int evt) { return 0; }
};

