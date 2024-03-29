#pragma once

#include "edit_tool.h"
#include "../objects/polygon.h"


class PolygonTool : public EditTool {
public:
    PolygonTool(MainWindow *mw) : EditTool() { m_mw = mw; }
    // TODO: Write destructor
    ~PolygonTool() { }
    // Poly *create_polygon();
    Shape *begin_shape_handle() override;
    void end_shape_handle() override;
    int create_main_handle(int evt) override;
    // TODO(daniel): Implement edit_main_handle(int)
    int edit_main_handle(int evt) override { return 0; };
    // void draw_nodes();
    // void draw_outline();
private:
    int keyboard_handle(int evt);
    int mouse_handle(int evt);
    Poly *m_temp_polygon;
    Vec2f m_mouse_world;
    Vec2f m_mouse_world_snap;
    Vec2f *m_active_point;
};