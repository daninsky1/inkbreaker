#include "polygon_tool.h"
#include "../view2d.h"


Shape *PolygonTool::begin_shape_handle()
{
    m_temp_polygon = new Poly();
    return dynamic_cast<Shape*>(m_temp_polygon);
}

void PolygonTool::end_shape_handle()
{
    if (m_temp_polygon->size() <= 2) {
        delete m_temp_polygon;
    }
    else {
        m_temp_polygon->pop_back();
        Tree::Node *wrapper = new Tree::Node((Shape*)m_temp_polygon);
        m_mw->root->add_child(wrapper);
    }
    m_active_point = nullptr;
    m_temp_polygon = nullptr;
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
            end_operation();
        }
        m_mw->v2d->redraw();
        handled = 1;
    } break;
    case FL_Escape: {
        if (is_in_operation()) {
            m_temp_polygon->clear();
            end_operation();
            m_mw->v2d->redraw();
            handled = 1;
        }
        handled = 1;
    } break;
    case FL_BackSpace: {
        if (is_in_operation()) {
            if (m_temp_polygon->size() > 2) {
                m_temp_polygon->pop_back();
                m_active_point = m_temp_polygon->last_point();
                
                m_mw->v2d->get_mouse_v2d_to_world_position(&m_mouse_world);
                m_mouse_world_snap = m_mouse_world;
                m_mw->v2d->get_snap(&m_mouse_world_snap); 
                
                *m_active_point = m_mouse_world_snap;
                
                m_mw->v2d->redraw();
                handled = 1;
            }
        }
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
            
            m_mw->v2d->redraw();
            handled = 1;
        }
    } break;
    case FL_DRAG: {
        if (is_in_operation() && (Fl::event_button() == FL_LEFT_MOUSE)) {
            m_mw->v2d->get_mouse_v2d_to_world_position(&m_mouse_world);
            m_mouse_world_snap = m_mouse_world;
            m_mw->v2d->get_snap(&m_mouse_world_snap);
            
            *m_active_point = m_mouse_world_snap;
            
            m_mw->v2d->redraw();
            handled = 1;
        }
    } break;
    case FL_RELEASE: {
        m_mw->v2d->get_mouse_v2d_to_world_position(&m_mouse_world);
        m_mouse_world_snap = m_mouse_world;
        m_mw->v2d->get_snap(&m_mouse_world_snap);

        if (Fl::event_button() == FL_LEFT_MOUSE) {
            // first node at location of left click
            if (!is_in_operation()) {
                begin_operation();
                m_temp_polygon->add_point(m_mouse_world_snap);
                // m_temp_polygon->shape_info = m_mw->v2d->shape_info; // Update to new ShapeInfo
            }
            m_active_point = m_temp_polygon->add_point(m_mouse_world_snap);
        }
        m_mw->v2d->redraw();
        handled = 1;
    } break;
    default: {
    }
    }
    return handled;
}
