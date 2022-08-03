#include "polygon_tool.h"


bool EditTool::m_is_active = false;
EditTool EditTool::*m_active_tool = nullptr;

void EditTool::is_active(bool ia)
{
    assert(!m_is_active);
    m_is_active = ia;
}

bool EditTool::is_active()
{
    return m_is_active;
}

Polygon *create_polygon()
{
    return new Polygon();
}

int PolygonTool::create_polygon_handle(int evt)
{
    int handled = 0;
    return handled;
}

int PolygonTool::edit_polygon_handle(int evt)
{
    
}

class CreateBezierHandle : public EditTool {
    int create_bezier(int evt ) { return 0; }
    int edit_bezier(int evt) { return 0; }
};

