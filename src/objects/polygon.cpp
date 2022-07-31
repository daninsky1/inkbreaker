#include "polygon.h"

#include <FL/fl_draw.H>


void Polygon::draw()
{
    int px, py;

    fl_color(shape_info.fill_color);
    fl_begin_polygon();
    for (size_t i = 0; i < points.size(); ++i) {
        world_to_scr(points[i], px, py);
        fl_vertex(px, py);
    }
    fl_end_polygon();

    fl_color(shape_info.line_color);
    fl_line_style(FL_SOLID | FL_JOIN_MITER, shape_info.line_width*(int)world_scale);
    fl_begin_line();
    for (size_t i = 0; i < points.size(); ++i) {
        world_to_scr(points[i], px, py);
        fl_vertex(px, py);
    }
    fl_end_line();
}

Vec2f *Polygon::add_point(const Vec2f p)
{
    points.push_back(p);
    return &points[points.size() - 1];
}

Vec2f *Polygon::insert_point(int index, const Vec2f p)
{
    vector<Vec2f>::iterator it = points.begin() + index;
    points.insert(it, p);
    return &points[index];
}
