#include "bezier_tool.h"
#include "../view2d.h"


Shape *BezierTool::begin_shape_handle()
{
    return dynamic_cast<Shape*>(new Bezier());
}

void BezierTool::end_shape_handle()
{

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
    // int key_code = Fl::event_key();
    
    // switch (key_code) {
    // case FL_Enter: {
    //     if (is_in_operation()) {
    //         if (m_temp_bezier->size() <= 2) {
    //             delete m_temp_bezier;
    //         }
    //         else {
    //             m_temp_bezier->pop_back();
    //             Tree::Node *wrapper = new Tree::Node((Shape*)m_temp_bezier);
    //             m_mw->root->add_child(wrapper);
    //         }
    //         m_active_bhandle = nullptr;
    //         end_operation(this);
    //         m_temp_bezier = nullptr;
    //     }
    //     m_mw->v2d->redraw();
    //     handled = 1;
    // } break;
    // case FL_Escape: {
    //     if (is_in_operation()) {
    //         end_operation(this);
    //         m_active_bhandle = nullptr;
    //         delete m_temp_bezier;
    //         m_temp_bezier = nullptr;
    //         m_mw->v2d->redraw();
    //     }
    //     handled = 1;
    // } break;
    // case FL_BackSpace: {
    //     if (is_in_operation()) {
    //         if (m_temp_bezier->size() <= 2) {
    //             end_operation(this);
    //             m_active_bhandle = nullptr;
    //             delete m_temp_bezier;
    //             m_temp_bezier = nullptr;
    //         }
    //         else {
    //             m_temp_bezier->pop_back();
    //             m_active_bhandle = m_temp_bezier->last_bhandle();
    //         }
    //         // TODO(daniel): Check if cursor update is necessary here
    //         // get_cursor_v2d_position(mouse_v2d.x, mouse_v2d.y);
    //         // scr_to_world(mouse_v2d.x, mouse_v2d.y, mouse_world);
    //         // mouse_snap_world = get_snap_grid(mouse_world);
    //         // world_to_scr(mouse_snap_world, snap_cursor_v2d.x, snap_cursor_v2d.y);
    //         m_mw->v2d->redraw();
    //     }
    //     handled = 1;
    // } break;
    // }
    return handled;
}

int BezierTool::mouse_handle(int evt)
{
    int handled = 0;
    return handled; 
}
