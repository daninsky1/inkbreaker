#include "edit_tools.h"


EditTool EditTool::*m_active_tool = nullptr;
EditTool EditTool::*m_m_temp_polygon = nullptr;

void EditTool::activate_tool(EditTool *active_tool)
{
    assert(!m_active_tool);
    if (m_active_tool); // throw?;
    m_active_tool = active_tool;
}

void EditTool::deactivate_tool(EditTool *et_to_deactivate)
{
    assert(et_to_deactivate == m_active_tool);
    if (et_to_deactivate != m_active_tool); // throw?;
    m_active_tool = nullptr;
}

void EditTool::temp_shape(EditTool *active_et, Shape *temp_shape)
{
    assert(active_et == m_active_tool);
    m_temp_shape == temp_shape;
}

void EditTool::is_in_operation(EditTool *active_et, bool is_in_operation)
{
    assert(active_et == m_active_tool);
    m_is_in_operation = is_in_operation;
}

Polygon *create_polygon()
{
    return new Polygon();
}

PolygonTool::PolygonTool(MainWindow *mw) {
    m_mw = mw;
    activate_tool(this);
    m_temp_polygon = new Polygon();
    temp_shape(this, dynamic_cast<Shape*>(m_temp_polygon));
}

int PolygonTool::create_main_handle(int evt)
{
    int handled = 0;

    if (evt == FL_KEYBOARD) {
        handled = keyboard_handle(evt);
        if (handled) return handled;
    }
    handled = mouse_handle(evt);
    if (handled) return handled;

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
                is_active = false;
                m_active_point = nullptr;
                delete m_temp_polygon;
                m_temp_polygon = nullptr;
            }
            else {
                is_active = false;
                m_temp_polygon->nodes.pop_back();
                shapes.push_back(m_temp_polygon);
                m_active_point = nullptr;
                m_temp_polygon = nullptr;
            }
        }
        m_mw->v2d->redraw();
        handled = 1;
    } break;
    case FL_Escape: {
        if (is_active) {
            is_active = false;
            m_active_point = nullptr;
            delete m_temp_polygon;
            m_temp_polygon = nullptr;
            m_mw->v2d->redraw();
        }
        if (is_selecting) {

        }
        handled = 1;
    } break;
    case FL_BackSpace: {
        if (is_active) {
            if (m_temp_polygon) {
                if (m_temp_polygon->nodes.size() <= 2) {
                    is_active = false;
                    m_active_point = nullptr;
                    delete m_temp_polygon;
                    m_temp_polygon = nullptr;
                }
                else {
                    m_temp_polygon->nodes.pop_back();
                    m_active_point = &m_temp_polygon->nodes[m_temp_polygon->nodes.size() - 1];
                    printf("nodes size %lu\n", m_temp_polygon->nodes.size());
                }
            }
            else if (bezier_m_temp_polygon) {
                if (bezier_m_temp_polygon->bhandles.size() < 2) {
                    is_active = false;
                    active_bhandle = nullptr;
                    delete bezier_m_temp_polygon;
                    bezier_m_temp_polygon = nullptr;
                }
                else {
                    bezier_m_temp_polygon->bhandles.pop_back();
                }
            }

            get_cursor_v2d_position(mouse_v2d.x, mouse_v2d.y);
            scr_to_world(mouse_v2d.x, mouse_v2d.y, mouse_world);
            mouse_snap_world = get_snap_grid(mouse_world);
            world_to_scr(mouse_snap_world, snap_cursor_v2d.x, snap_cursor_v2d.y);

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
        }
        // if (!m_active_point) {
        // }
        handled = 1;
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
        // scr_to_world(mouse_v2d.x, mouse_v2d.y, mouse_world);
        // mouse_snap_world = get_snap_grid(mouse_world);

        if (Fl::event_button() == FL_LEFT_MOUSE) {
            // first node at location of left click
            if (!is_in_operation()) {
                m_temp_polygon->add_point(m_mouse_world);
                is_in_operation(this, true);
            }
            m_active_point = m_temp_polygon->add_point(m_mouse_world);

            
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

class CreateBezierHandle : public EditTool {
    int create_bezier(int evt ) { return 0; }
    int edit_bezier(int evt) { return 0; }
};

