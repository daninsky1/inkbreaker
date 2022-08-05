#include "edit_tool.h"

#include <FL/names.h>


EditTool *EditTool::m_active_tool = nullptr;
Shape *EditTool::m_temp_shape = nullptr;

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

EditTool *EditTool::active_tool()
{
    return m_active_tool;
};

bool EditTool::is_active()
{
    return m_active_tool ? true : false;
};

void EditTool::temp_shape(EditTool *active_et, Shape *temp_shape)
{
    assert(active_et == m_active_tool);
    m_temp_shape == temp_shape;
}

Shape *EditTool::temp_shape()
{
    return m_temp_shape;
}

bool EditTool::is_in_operation()
{
    return m_temp_shape ? true : false;
};

void EditTool::begin_operation(EditTool *active_et, Shape *temp_shape)
{
    assert(active_et == m_active_tool);
    assert(!m_temp_shape);
    m_temp_shape = dynamic_cast<Shape*>(temp_shape);
}

void EditTool::end_operation(EditTool *active_et)
{
    assert(active_et == m_active_tool);
    m_temp_shape = nullptr;
}





class CreateBezierHandle : public EditTool {
    int create_bezier(int evt ) { return 0; }
    int edit_bezier(int evt) { return 0; }
};

