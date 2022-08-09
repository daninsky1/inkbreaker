#include "edit_tool.h"
#include "polygon_tool.h"
#include "bezier_tool.h"

#include <FL/names.h>


EditTool *EditTool::m_active_tool = nullptr;
Shape *EditTool::m_temp_shape = nullptr;

/* A factory function to create or change EditTool */
EditTool *EditTool::edit_tool(Draw dmode, MainWindow *mw)
{
    if (m_active_tool) {
        if (m_temp_shape) {
            end_operation();
        }
        delete m_active_tool;
    }
    switch (dmode) {
    case Draw::polygon: {
        m_active_tool = new PolygonTool(mw);
    } break;
    case Draw::bezier: {
        m_active_tool = new BezierTool(mw);
    } break;
    default:
        assert(!"Type unhandled.");
    }
    return m_active_tool;
}

bool EditTool::is_active()
{
    return m_active_tool ? true : false;
};

Shape *EditTool::temp_shape()
{
    return m_temp_shape;
}

bool EditTool::is_in_operation()
{
    return m_temp_shape ? true : false;
};

void EditTool::begin_operation()
{
    assert(!m_temp_shape);
    m_temp_shape = m_active_tool->begin_shape_handle();
}

void EditTool::end_operation()
{
    assert(m_temp_shape);
    m_active_tool->end_shape_handle();
    m_temp_shape = nullptr;
}
