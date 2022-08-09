#include "bezier_tool.h"
#include "../view2d.h"


Shape *BezierTool::begin_shape_handle()
{
    m_temp_bezier = new Bezier();
    return m_temp_bezier;
}

void BezierTool::end_shape_handle()
{
    if (m_temp_bezier->size() <= 2) {
        delete m_temp_bezier;
    }
    else {
        m_temp_bezier->pop_back();
        Tree::Node *wrapper = new Tree::Node((Shape*)m_temp_bezier);
        m_mw->root->add_child(wrapper);
    }
    m_active_bhandle = nullptr;
    m_temp_bezier = nullptr; 
}

int BezierTool::create_main_handle(int evt)
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

int BezierTool::keyboard_handle(int evt)
{
    int handled = 0;
    int key_code = Fl::event_key();
    
    switch (key_code) {
    case FL_Enter: {
        if (is_in_operation()) {
            end_operation();
            m_mw->v2d->redraw();
            handled = 1;
        }
    } break;
    case FL_Escape: {
        if (is_in_operation()) {
            m_temp_bezier->clear();
            end_operation();
            m_mw->v2d->redraw();
            handled = 1;
        }
    } break;
    case FL_BackSpace: {
        if (is_in_operation()) {
            if (m_temp_bezier->size() > 2) {
                m_temp_bezier->pop_back();
                m_active_bhandle = m_temp_bezier->last_bhandle();
                
                m_mw->v2d->get_mouse_v2d_to_world_position(&m_mouse_world);
                m_mouse_world_snap = m_mouse_world;
                m_mw->v2d->get_snap(&m_mouse_world_snap);
                
                m_active_bhandle->point = m_mouse_world_snap;
                m_active_bhandle->head = m_mouse_world_snap;
                m_active_bhandle->tail = m_mouse_world_snap;
                
                m_mw->v2d->redraw();
                handled = 1;
            }
        }
    } break;
    }
    return handled;
}

int BezierTool::mouse_handle(int evt)
{
    int handled = 0;
    
    switch (evt) {
    case FL_MOVE: {
        if (is_in_operation()) {
            m_mw->v2d->get_mouse_v2d_to_world_position(&m_mouse_world);
            m_mouse_world_snap = m_mouse_world;
            m_mw->v2d->get_snap(&m_mouse_world_snap);
            
            m_active_bhandle->point = m_mouse_world_snap;
            m_active_bhandle->head = m_mouse_world_snap;
            m_active_bhandle->tail = m_mouse_world_snap;
            m_mw->v2d->redraw();
            handled = 1;
        }
    } break;
    case FL_PUSH: {
        if (!is_in_operation() && Fl::event_button() == FL_LEFT_MOUSE) {
            /* First BezierHandle */
            // NOTE(daniel): Bezier curve starts to draw at left mouse button,
            // unlike the other shapes, because has the drag feature to move
            // the bezier handles
            m_mw->v2d->get_mouse_v2d_to_world_position(&m_mouse_world);
            m_mouse_world_snap = m_mouse_world;
            m_mw->v2d->get_snap(&m_mouse_world_snap);
            
            begin_operation();
            m_active_bhandle = m_temp_bezier->add_bhandle(m_mouse_world_snap, m_mouse_world_snap, m_mouse_world_snap);   // First BezierHandle
            // TODO: Add ShapeInfo in the creation of temp_shape
            // m_temp_bezier->shape_info = m_mw->v2d->shape_info; // Update to new ShapeInfo
            
            m_mw->v2d->redraw();
            handled = 1;
        }
    } break;
    case FL_DRAG: {
        /* NOTE: When dragging the BezierHandle point anchor on the last click
        spot, becuse when this event begin blocks FL_MOVE event, here we only
        need to update BezierHandle head and tail */
        if (is_in_operation() && (Fl::event_button() == FL_LEFT_MOUSE)) {
            m_mw->v2d->get_mouse_v2d_to_world_position(&m_mouse_world);
            m_mouse_world_snap = m_mouse_world;
            m_mw->v2d->get_snap(&m_mouse_world_snap);
            
            m_active_bhandle->head = m_mouse_world;
            // Mouse update head and mirror to tail
            m_active_bhandle->tail.x = m_active_bhandle->point.x + (m_active_bhandle->point.x - m_active_bhandle->head.x);
            m_active_bhandle->tail.y = m_active_bhandle->point.y + (m_active_bhandle->point.y - m_active_bhandle->head.y);
            m_mw->v2d->redraw();
            handled = 1;
        }
    } break;
    case FL_RELEASE: {
        if (Fl::event_button() == FL_LEFT_MOUSE) {
            m_mw->v2d->get_mouse_v2d_to_world_position(&m_mouse_world);
            m_mouse_world_snap = m_mouse_world;
            m_mw->v2d->get_snap(&m_mouse_world_snap);
            
            m_active_bhandle = m_temp_bezier->add_bhandle(m_mouse_world_snap, m_mouse_world_snap, m_mouse_world_snap);   // First BezierHandle
            
            // TODO: Close Bezier
            // if (!active_point) {
            //     shapes.push_back(m_temp_bezier);
            //     printf("Shape stored: %s\n", m_temp_bezier->type().c_str());
            //     printf("%f, %f - %f, %f\n", m_temp_bezier->nodes[0].pos.x, m_temp_bezier->nodes[0].pos.y, m_temp_bezier->nodes[1].pos.x, m_temp_bezier->nodes[1].pos.y);
            //     is_drawing = false;
            //     changed = true;
            //     m_temp_bezier = nullptr;
            //     active_point = nullptr;
            // }
            m_mw->v2d->redraw(); 
            handled = 1;
        }
    } break;
    default: {
    }
    }
    
    return handled; 
}
