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

sSelectBox::sSelectBox()
{
    max_nodes = 2;
    // VECTOR NEEDS TO HAVE SIZE PREDEFINED SEE: sShape::get_next_node definition
    nodes.reserve(max_nodes);
}

void sSelectBox::draw_shape()
{
    int sx, sy, ex, ey;
    world_to_scr(nodes[0].pos, sx, sy);
    world_to_scr(nodes[1].pos, ex, ey);

    // NOTE(daniel): Normalize start and end point to draw the rectangle
    // correctly
    int normalsx = sx;
    int normalsy = sy;
    int normalex = ex;
    int normaley = ey;

    // TODO(daniel): Add autocad inspired colors to the line selections
    fl_color(FL_WHITE);
    // NOTE(daniel): Join miter doesn't work with fl_rect, but here it's not
    // necessery

    if (ex < sx) {
        // Window selection line style
        fl_line_style(FL_DASH, 1);
        normalsx = ex;
        normalex = sx;
    }
    else {
        // Cross selection line style
        fl_line_style(FL_SOLID, 1);
    }
    if (ey < sy) {
        normalsy = ey;
        normaley = sy;
    }

    fl_rect(normalsx, normalsy, abs(normalex-normalsx), abs(normaley-normalsy));
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

void sLine::draw_bbox()
{
    int sx, sy, ex, ey;
    world_to_scr(bboxs, sx, sy);
    world_to_scr(bboxe, ex, ey);

    fl_color(FL_WHITE);
    fl_line_style(FL_DASH | FL_JOIN_MITER, 1);
    fl_begin_loop();
    fl_vertex(sx, sy); fl_vertex(ex, sy);
    fl_vertex(ex, ey); fl_vertex(sx, ey);
    fl_vertex(sx, sy);
    fl_end_loop();
}

void sLine::update_bbox()
{
    if (nodes[0].pos.x < nodes[1].pos.x) {
        bboxs.x = nodes[0].pos.x;
        bboxe.x = nodes[1].pos.x;
    }
    else {
        bboxs.x = nodes[1].pos.x;
        bboxe.x = nodes[0].pos.x;
    }
    if (nodes[0].pos.y > nodes[1].pos.y) {
        bboxs.y = nodes[0].pos.y;
        bboxe.y = nodes[1].pos.y;
    }
    else {
        bboxs.y = nodes[1].pos.y;
        bboxe.y = nodes[0].pos.y;
    }
}

sRect::sRect()
{
    max_nodes = 2;
    // VECTOR NEEDS TO HAVE SIZE PREDEFINED SEE: sShape::get_next_node definition
    nodes.reserve(max_nodes);
}

void sRect::update_bbox()
{
    bboxs.x = nodes[0].pos.x;
    bboxs.y = nodes[0].pos.y;
    bboxe.x = nodes[1].pos.x;
    bboxe.y = nodes[1].pos.y;
}

void sRect::draw_shape()
{
    int sx, sy, ex, ey;
    world_to_scr(nodes[0].pos, sx, sy);
    world_to_scr(nodes[1].pos, ex, ey);

    // BUG(daniel): The order that this is drawn causes problems if the user
    // do the drawing in the oposite order

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

void sRect::draw_bbox()
{
    int sx, sy, ex, ey;
    world_to_scr(bboxs, sx, sy);
    world_to_scr(bboxe, ex, ey);

    fl_color(FL_WHITE);
    fl_line_style(FL_DASH | FL_JOIN_MITER, 1);
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

void sCircle::update_bbox()
{
    int sx, sy, ex, ey;
    world_to_scr(nodes[0].pos, sx, sy);
    world_to_scr(nodes[1].pos, ex, ey);
    //float r = sqrtf((float)pow(sx-ex, 2) + (float)pow(sy-ey, 2));
    float r = sqrtf((float)pow(nodes[0].pos.x-nodes[1].pos.x, 2) + (float)pow(nodes[0].pos.y-nodes[1].pos.y, 2));

    bboxs.x = nodes[0].pos.x - r;
    bboxs.y = nodes[0].pos.y - r;
    bboxe.x = nodes[0].pos.x + r;
    bboxe.y = nodes[0].pos.y + r;
}

void sCircle::draw_shape()
{
    int sx, sy, ex, ey;
    world_to_scr(nodes[0].pos, sx, sy);
    world_to_scr(nodes[1].pos, ex, ey);
    float r = sqrtf((float)pow(sx-ex, 2) + (float)pow(sy-ey, 2));

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

void sCircle::draw_bbox()
{
    int sx, sy, ex, ey;
    world_to_scr(bboxs, sx, sy);
    world_to_scr(bboxe, ex, ey);

    fl_color(FL_WHITE);
    fl_line_style(FL_DASH | FL_JOIN_MITER, 1);
    fl_begin_loop();
    fl_vertex(sx, sy); fl_vertex(ex, sy);
    fl_vertex(ex, ey); fl_vertex(sx, ey);
    fl_vertex(sx, sy);
    fl_end_loop();
}

