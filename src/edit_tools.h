/* This draw and edit by mouse input for now */
#pragma once

#include "assert.h"

#include <FL/Enumerations.H>

#include "main_window.h"
#include "enumerators.h"
#include "coordinates.h"
#include "objects/polygon.h"
#include "tree.h"


// TODO(daniel): prevent instanciation of EditTool
class EditTool {
public:
    EditTool() { }
    static void activate_tool(EditTool *active_tool);
    static void deactivate_tool(EditTool *et_to_deactivate);
    static EditTool *active_tool();
    static bool is_active();
    static void temp_shape(EditTool *active_et, Shape *temp_shape);
    static Shape *temp_shape();
    /* Check if is in the middle of a unfinished drawing */
    static void begin_operation(EditTool *et, Shape *temp_shape);
    static void end_operation(EditTool *et);
    static bool is_in_operation();
    virtual int create_main_handle(int evt) {};
    virtual int edit_main_handle(int evt) {};
protected:
    MainWindow *m_mw;
private:
    static EditTool *m_active_tool;
    static Shape *m_temp_shape;
    /* NOTE(daniel): Check if EditTool is in the middle of an operation */
};

class PolygonTool : public EditTool {
public:
    PolygonTool(MainWindow *mw);
    int create_main_handle(int evt) override;
    // TODO(daniel): Implement edit_main_handle(int)
    int edit_main_handle(int evt) override { };
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
