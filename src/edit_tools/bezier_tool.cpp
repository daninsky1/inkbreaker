#include "bezier.h"


BezierTool::BezierTool(MainWindow *mw)
{
    m_mw = mw;
    activate_tool(this); 
}

Bezier *BezierTool::create_bezier()
{
    return new Bezier();
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
    return handled;
}

int BezierTool::mouse_handle(int evt)
{
    int handled = 0;
    return handled; 
}
