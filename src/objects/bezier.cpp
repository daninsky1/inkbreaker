#include "bezier.h"

#include <FL/fl_draw.H>


void Bezier::draw()
{
    //printf("v_size: %zd\n", nodes.size());

    int px0, py0, px1, py1;     // Curve points
    int hx0, hy0, hx1, hy1;     // Curve handles

    fl_color(shape_info.fill_color);
    fl_begin_polygon();
    for (size_t i = 0; i < bhandles.size() - 1; ++i) {
        world_to_scr(bhandles[i].point, px0, py0);
        world_to_scr(bhandles[i].head, hx0, hy0);
        world_to_scr(bhandles[i+1].tail, hx1, hy1);
        world_to_scr(bhandles[i+1].point, px1, py1);
        fl_curve(px0, py0, hx0, hy0, hx1, hy1, px1, py1);
    }
    fl_end_polygon();

    fl_begin_line();
    fl_color(shape_info.line_color);
    fl_line_style(FL_SOLID | FL_JOIN_MITER, shape_info.line_width*(int)world_scale);
    for (size_t i = 0; i < bhandles.size() - 1; ++i) {
        world_to_scr(bhandles[i].point, px0, py0);
        world_to_scr(bhandles[i].head, hx0, hy0);
        world_to_scr(bhandles[i+1].tail, hx1, hy1);
        world_to_scr(bhandles[i+1].point, px1, py1);
        fl_curve(px0, py0, hx0, hy0, hx1, hy1, px1, py1);
    }
    fl_end_line();
}

BezierHandle *Bezier::add_point(const BezierHandle bh)
{
    bhandles.push_back(bh);
    return &bhandles[bhandles.size() - 1];
}
BezierHandle *Bezier::add_point(Vec2f point, Vec2f head, Vec2f tail)
{
    add_point(BezierHandle{point, head, tail});
}

BezierHandle *Bezier::insert_point(int index, const BezierHandle p)
{
    vector<BezierHandle>::iterator it = bhandles.begin() + index;
    bhandles.insert(it, p);
    return &bhandles[index];
}
