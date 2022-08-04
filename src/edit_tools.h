/* This draw and edit by mouse input for now */
#pragma once

#include "assert.h"

#include <FL/Enumerations.H>

#include "main_window.h"
#include "view2d.h"
#include "enumerators.h"
#include "coordinates.h"
#include "objects/polygon.h"


// TODO(daniel): prevent instanciation of EditTool
class EditTool {
public:
    static void activate_tool(EditTool *active_tool);
    static void deactivate_tool(EditTool *et_to_deactivate);
    static EditTool *active_tool() { return m_active_tool; };
    static bool is_active() { return m_active_tool ? true : false; };
    static void temp_shape(EditTool *active_et, Shape *temp_shape);
    static Shape *temp_shape() { return m_temp_shape; }
    static void is_in_operation(EditTool *et, bool is_in_operation);
    static bool is_in_operation() { return m_is_in_operation; };
    virtual int create_main_handle(int evt) = 0;
    virtual int edit_main_handle(int evt) = 0;
protected:
    MainWindow *m_mw;
private:
    static EditTool *m_active_tool;
    static Shape *m_temp_shape;
    /* NOTE(daniel): Check if EditTool is in the middle of an operation */
    static bool m_is_in_operation;
};

class PolygonTool : public EditTool {
public:
    PolygonTool(MainWindow *mw);
    int create_main_handle(int evt) override;
    int edit_main_handle(int evt) override;
    // void draw_nodes();
    // void draw_outline();
private:
    int keyboard_handle(int evt);
    int mouse_handle(int evt);
    Polygon *m_temp_polygon;
    Vec2f m_mouse_world;
    Vec2f m_mouse_world_snap;
    Vec2f *m_active_point;
};
