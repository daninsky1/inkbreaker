#pragma once

#include "edit_tool.h"
#include "../objects/polygon.h"


class PolygonTool : public EditTool {
public:
    PolygonTool(MainWindow *mw);
    // Polygonn *create_polygon();
    int create_main_handle(int evt) override;
    // TODO(daniel): Implement edit_main_handle(int)
    int edit_main_handle(int evt) override { return 0; };
    // void draw_nodes();
    // void draw_outline();
private:
    int keyboard_handle(int evt);
    int mouse_handle(int evt);
    Polygonn *m_temp_polygon;
    Vec2f m_mouse_world;
    Vec2f m_mouse_world_snap;
    Vec2f *m_active_point;
};