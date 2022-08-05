#include "edit_tool.h"

#include <FL/names.h>


EditTool *EditTool::m_active_tool = nullptr;
Shape *EditTool::m_temp_shape = nullptr;

EditTool::EditTool(MainWindow *mw, EditTool *et)
{
    assert(!m_active_tool);
    if (m_active_tool); // throw?;
    
    m_mw = mw;
    if (is_active()) { free_edit_tool(); }
    m_active_tool = et;
}

void EditTool::free_edit_tool()
{
    if (m_active_tool) {
        if (is_in_operation()) {
            m_active_tool->end_operation();
        }
        delete m_active_tool;
        m_active_tool = nullptr;
    };
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

void EditTool::begin_operation(Shape *temp_shape)
{
    assert(!m_temp_shape);
    m_temp_shape = temp_shape;
}

void EditTool::end_operation()
{
    assert(m_temp_shape);
    m_active_tool->register_shape();
    m_temp_shape = nullptr;
}
