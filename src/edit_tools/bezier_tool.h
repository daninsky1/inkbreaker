#include "edit_tool.h"
#include "../objects/bezier.h"


class BezierTool : public EditTool {
public:
    BezierTool(MainWindow *mw);
    Bezier *create_bezier();
    int create_main_handle(int evt) override;
    // TODO(daniel): Implement edit_main_handle(int)
    int edit_main_handle(int evt) override { };
    // void draw_nodes();
    // void draw_outline();
private:
    int keyboard_handle(int evt);
    int mouse_handle(int evt);
    Bezier *m_temp_bezier;
    Vec2f m_mouse_world;
    Vec2f m_mouse_world_snap;
    Vec2f *m_active_point;
};