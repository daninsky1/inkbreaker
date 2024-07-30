#pragma once

#include "edit_tool.h"
#include "../objects/bezier.h"


class BezierTool : public EditTool {
public:
    BezierTool(MainWindow *mw) : EditTool() { m_mw = mw; }
    // TODO: Write destructor
    ~BezierTool() { }
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
    Bezier *m_temp_bezier = nullptr;
    Vec2f m_mouse_world;
    Vec2f m_mouse_world_snap;
    BezierHandle *m_active_bhandle = nullptr;
    bool m_is_dragging = false;
};
