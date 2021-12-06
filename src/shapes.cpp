#include "shapes.h"


std::ostream& operator<<(std::ostream& os, const Vector& v)
{
    return os << '[' << v.x
        << ", " << v.y << ']';
}

//std::ostream& operator<<(std::ostream& os, const Line& line)
//{
//	return os << line.get_a() << ", " << line.get_b() << " ----";
//}


double sShape::world_scale{ 1.0 };
Vector sShape::world_offset{ 0.0, 0.0 };

sNode* sShape::get_next_node(const Vector& p)
{
    if (nodes.size() == max_nodes) return nullptr;
    sNode n;
    n.parent = this;
    n.pos = p;
    nodes.push_back(n);
    // WARNING THIS POINTER POINTS TO A STATIC SIZED VECTOR
    return &nodes[nodes.size() - 1];
}

void sShape::draw_nodes()
{
    for (auto& n : nodes) {
        int sx, sy;
        world_to_scr(n.pos, sx, sy);
        fl_line_style(FL_SOLID, 1);
        fl_color(FL_MAGENTA);
        fl_circle(sx, sy, 2);
    }
}

void sShape::world_to_scr(Vector& v, int& scrx, int& scry)
{
    scrx = static_cast<int>((v.x - world_offset.x) * world_scale);
    scry = static_cast<int>((v.y - world_offset.y) * world_scale);
}

sLine::sLine()
{
    max_nodes = 2;
    // VECTOR NEEDS TO HAVE SIZE PREDEFINED SEE: sShape::get_next_node definition
    nodes.reserve(max_nodes);
}

void sLine::draw_shape()
{
    int sx, sy, ex, ey;
    world_to_scr(nodes[0].pos, sx, sy);
    world_to_scr(nodes[1].pos, ex, ey);
    fl_color(sinfo.line_color);
    fl_line_style(FL_SOLID, sinfo.line_width*(int)world_scale);
    //fl_line_style(FL_SOLID, 2);
    fl_line(sx, sy, ex, ey);
}

sRect::sRect()
{
    max_nodes = 2;
    // VECTOR NEEDS TO HAVE SIZE PREDEFINED SEE: sShape::get_next_node definition
    nodes.reserve(max_nodes);
}

void sRect::draw_shape()
{
    int sx, sy, ex, ey;
    world_to_scr(nodes[0].pos, sx, sy);
    world_to_scr(nodes[1].pos, ex, ey);

    fl_color(sinfo.fill_color);
    fl_rectf(sx, sy, ex-sx, ey-sy, sinfo.fill_color);

    // NOTE(daniel): The straing foward fl_rect() do not apply the miter state.
    // The work around is pretty simple use line loop.

    fl_color(sinfo.line_color);
    fl_line_style(FL_SOLID | FL_JOIN_MITER, sinfo.line_width*(int)world_scale);
    fl_begin_loop();
    fl_vertex(sx, sy); fl_vertex(ex, sy);
    fl_vertex(ex, ey); fl_vertex(sx, ey);
    fl_vertex(sx, sy);
    fl_end_loop();
}

sCircle::sCircle()
{
    max_nodes = 2;
    nodes.reserve(max_nodes);
}

void sCircle::draw_shape()
{
    int sx, sy, ex, ey;
    world_to_scr(nodes[0].pos, sx, sy);
    world_to_scr(nodes[1].pos, ex, ey);
    float r = sqrtf((float)pow((sx-ex), 2) + (float)pow((sy-ey), 2));

    // NOTE(daniel): FLTK seems to have a bug with color assigning stores fill
    // color state bleeds to the fl_circle() shape that has no fill. The work
    // around is pretty simple use loop to do the line.

    fl_color(sinfo.fill_color);
    fl_begin_polygon();
    fl_arc((float)sx, (float)sy, r, 0, 360);
    fl_end_polygon();

    fl_color(sinfo.line_color);
    fl_line_style(FL_SOLID | FL_JOIN_MITER, sinfo.line_width*(int)world_scale);
    fl_begin_loop();
    fl_arc((float)sx, (float)sy, r, 0, 360);
    fl_end_loop();
}

