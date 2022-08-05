#include "polygon_tool.h"
#include "../view2d.h"


PolygonTool::PolygonTool(MainWindow *mw)
{
    m_mw = mw;
    activate_tool(this);
}

Polygon *PolygonTool::create_polygon()
{
    return new Polygon();
}

int PolygonTool::create_main_handle(int evt)
{
    int handled = 0;

    switch (evt) {
    case FL_KEYBOARD: {
        handled = keyboard_handle(evt);
    } break;
    default: {
        handled = mouse_handle(evt);
    }
    }
    // printf("Event was %s (%d), handled=%d\n", fl_eventnames[evt], evt, handled);
    return handled;
}

int PolygonTool::keyboard_handle(int evt)
{
    int handled = 0;
    int key_code = Fl::event_key();
    
    switch (key_code) {
    case FL_Enter: {
        if (is_in_operation()) {
            if (m_temp_polygon->size() <= 2) {
                delete m_temp_polygon;
            }
            else {
                m_temp_polygon->pop_back();
                Tree::Node *wrapper = new Tree::Node((Shape*)m_temp_polygon);
                m_mw->root->add_child(wrapper);
            }
            m_active_point = nullptr;
            end_operation(this);
            m_temp_polygon = nullptr;
        }
        m_mw->v2d->redraw();
        handled = 1;
    } break;
    case FL_Escape: {
        if (is_in_operation()) {
            end_operation(this);
            m_active_point = nullptr;
            delete m_temp_polygon;
            m_temp_polygon = nullptr;
            m_mw->v2d->redraw();
        }
        handled = 1;
    } break;
    case FL_BackSpace: {
        if (is_in_operation()) {
            if (m_temp_polygon->size() <= 2) {
                end_operation(this);
                m_active_point = nullptr;
                delete m_temp_polygon;
                m_temp_polygon = nullptr;
            }
            else {
                m_temp_polygon->pop_back();
                m_active_point = m_temp_polygon->last_point();
            }
            // TODO(daniel): Check if cursor update is necessary here
            // get_cursor_v2d_position(mouse_v2d.x, mouse_v2d.y);
            // scr_to_world(mouse_v2d.x, mouse_v2d.y, mouse_world);
            // mouse_snap_world = get_snap_grid(mouse_world);
            // world_to_scr(mouse_snap_world, snap_cursor_v2d.x, snap_cursor_v2d.y);
            m_mw->v2d->redraw();
        }
        handled = 1;
    } break;
    }
    return handled;
}

int PolygonTool::mouse_handle(int evt)
{
    int handled = 0;
    
    switch (evt) {
    case FL_MOVE: {
        if (is_in_operation()) {
            m_mw->v2d->get_mouse_v2d_to_world_position(&m_mouse_world);
            m_mouse_world_snap = m_mouse_world;
            m_mw->v2d->get_snap(&m_mouse_world_snap);
            // TODO(daniel): Decide who's responsible for the mouse snap
            // View2D or EditTool or at application level
            // NOTE(daniel): Make redraw? View2D redraw everytime when the mouse
            //moves, so no need to redraw here
            // m_mw->v2d->redraw();
            *m_active_point = m_mouse_world_snap;
            Vec2f last_point = m_temp_polygon->points[m_temp_polygon->size()-1];
            printf("polygon size %lu, %f, %f\n", m_temp_polygon->size(), last_point.x, last_point.y);
            
            handled = 1;
            m_mw->v2d->redraw();
        }
        // if (!m_active_point) {
        // }
    } break;
    // TODO(daniel): Check if drag start is necessery
    // case FL_PUSH: {
    //     get_cursor_v2d_position(mouse_v2d.x, mouse_v2d.y);
    //     scr_to_world(mouse_v2d.x, mouse_v2d.y, mouse_world);
    //     mouse_snap_world = get_snap_grid(mouse_world);
    //     world_to_scr(mouse_snap_world, snap_cursor_v2d.x, snap_cursor_v2d.y);

    //     // Review if this is necessary
    //     drag_start_scr_x = Fl::event_x_root();
    //     drag_start_scr_y = Fl::event_y_root();

    //     //m_mw->v2d->redraw();
    //     handled = 1;
    // } break;
    // case FL_DRAG: {
    //     get_cursor_v2d_position(mouse_v2d.x, mouse_v2d.y);
    //     scr_to_world(mouse_v2d.x, mouse_v2d.y, mouse_world);
    //     mouse_snap_world = get_snap_grid(mouse_world);
    //     world_to_scr(mouse_snap_world, snap_cursor_v2d.x, snap_cursor_v2d.y);

    //     // NOTE(daniel): Find a way to track dragging state(is_dragging) that
    //     // is tied to this function/state
    //     int drag_update_scrx = Fl::event_x_root();
    //     int drag_update_scry = Fl::event_y_root();

    //     if (is_active && (Fl::event_button() == FL_LEFT_MOUSE)) {
    //         // Update active head bhandle and tail handle
    //         if (active_bhandle) {
    //             active_bhandle->head = mouse_current_world;
    //             // Invert mouse coordinates
    //             active_bhandle->tail.x = active_bhandle->point.x + (active_bhandle->point.x - active_bhandle->head.x);
    //             active_bhandle->tail.y = active_bhandle->point.y + (active_bhandle->point.y - active_bhandle->head.y);
    //             //printf("head: p0: %f, %f | h0: %f, %f | tail: h1: %f, %f | p1: %f, %f\n");
    //         }
    //         drag_start_scr_x = drag_update_scrx;
    //         drag_start_scr_y = drag_update_scry;

    //         m_mw->v2d->redraw();
    //         handled = 1;
    //     }
    // } break;
    case FL_RELEASE: {
        m_mw->v2d->get_mouse_v2d_to_world_position(&m_mouse_world);
        m_mouse_world_snap = m_mouse_world;
        m_mw->v2d->get_snap(&m_mouse_world_snap);

        if (Fl::event_button() == FL_LEFT_MOUSE) {
            // first node at location of left click
            if (!is_in_operation()) {
                m_temp_polygon = create_polygon();
                m_temp_polygon->add_point(m_mouse_world_snap);
                begin_operation(this, m_temp_polygon);
            }
            m_active_point = m_temp_polygon->add_point(m_mouse_world_snap);

            
            // else {
            //     printf("mouse release: %f, %f - %f, %f\n", mouse_world.x, mouse_world.y, mouse_snap_world.x, mouse_snap_world.y);

            //     if (bezier_m_temp_polygon) {
            //         active_bhandle = bezier_m_temp_polygon->get_next_handle(*active_bhandle);
            //     }
            //     else {
            //         m_active_point = m_temp_polygon->get_next_node(mouse_current_world);

            //         printf("nodes size %lu\n", m_temp_polygon->nodes.size());
            //         if (!m_active_point) {
            //             shapes.push_back(m_temp_polygon);
            //             printf("Shape stored: %s\n", m_temp_polygon->type().c_str());
            //             printf("%f, %f - %f, %f\n", m_temp_polygon->nodes[0].pos.x, m_temp_polygon->nodes[0].pos.y, m_temp_polygon->nodes[1].pos.x, m_temp_polygon->nodes[1].pos.y);
            //             is_active = false;
            //             changed = true;
            //             m_temp_polygon = nullptr;
            //             m_active_point = nullptr;
            //         }
            //     }
            // }
        }
        m_mw->v2d->redraw();
        handled = 1;
    } break;
    default: {
    }
    }
    return handled;
}