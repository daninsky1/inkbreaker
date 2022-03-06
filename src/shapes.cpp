#include "shapes.h"


std::ostream& operator<<(std::ostream& os, const Vector2f& v)
{
    return os << '[' << v.x
        << ", " << v.y << ']';
}

Vector2f Shape::world_offset{ 0.0f, 0.0f };
float Shape::world_scale{ 1.0f };

void translate(float offx, float offy, Shape *s)
{
    for (int i = 0; i < s->nodes.size(); ++i) {
        s->nodes[i].pos.x += offx;
        s->nodes[i].pos.y += offy;
    }
}

void scale(float factor, float centerx, float centery)
{
    // TODO
}

Node* Shape::get_next_node(const Vector2f p)
{
    if (nodes.size() == max_nodes) return nullptr;
    Node n;
    n.parent = this;
    n.pos = p;
    nodes.push_back(n);
    // WARNING THIS POINTER POINTS TO A STATIC SIZED VECTOR
    return &nodes[nodes.size() - 1];
}

void Shape::draw_nodes()
{
    for (auto& n : nodes) {
        int sx, sy;
        world_to_scr(n.pos, sx, sy);
        fl_line_style(FL_SOLID, 1);
        fl_color(FL_MAGENTA);
        fl_circle(sx, sy, 2);
    }
}

void Shape::world_to_scr(Vector2f v, int& scrx, int& scry)
{
    scrx = static_cast<int>((v.x - world_offset.x) * world_scale);
    scry = static_cast<int>((v.y - world_offset.y) * world_scale);
}

BBox::BBox()
{
    max_nodes = 2;
    // VECTOR NEEDS TO HAVE SIZE PREDEFINED SEE: Shape::get_next_node definition
    nodes.reserve(max_nodes);
}

void BBox::draw_shape()
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

    scrw = normalsx - normalex;
    scrh = normalsy - normaley;

    fl_rect(normalsx, normalsy, abs(normalex-normalsx), abs(normaley-normalsy));
}

Line::Line()
{
    max_nodes = 2;
    // VECTOR NEEDS TO HAVE SIZE PREDEFINED SEE: Shape::get_next_node definition
    nodes.reserve(max_nodes);
}

void Line::draw_shape()
{
    int sx, sy, ex, ey;
    world_to_scr(nodes[0].pos, sx, sy);
    world_to_scr(nodes[1].pos, ex, ey);
    fl_color(shape_info.line_color);
    fl_line_style(FL_SOLID, shape_info.line_width*(int)world_scale);
    //fl_line_style(FL_SOLID, 2);
    fl_line(sx, sy, ex, ey);
}

void Line::draw_bbox()
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

void Line::update_bbox()
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

Rect::Rect()
{
    max_nodes = 2;
    // VECTOR NEEDS TO HAVE SIZE PREDEFINED SEE: Shape::get_next_node definition
    nodes.reserve(max_nodes);
}

void Rect::update_bbox()
{
    bboxs.x = nodes[0].pos.x;
    bboxs.y = nodes[0].pos.y;
    bboxe.x = nodes[1].pos.x;
    bboxe.y = nodes[1].pos.y;
}

void Rect::draw_shape()
{
    int sx, sy, ex, ey;
    world_to_scr(nodes[0].pos, sx, sy);
    world_to_scr(nodes[1].pos, ex, ey);

    int normalsx = sx, normalsy = sy;
    int normalex = ex, normaley = ey;
    if (normalsx > normalex) {
        normalsx = ex;
        normalex = sx;
    }
    if (normalsy > normaley) {
        normalsy = ey;
        normaley = sy;
    }

    fl_color(shape_info.fill_color);
    fl_rectf(normalsx, normalsy, normalex-normalsx, normaley-normalsy, shape_info.fill_color);

    // NOTE(daniel): The straing foward fl_rect() do not apply the miter state.
    // The work around is pretty simple use line loop.

    fl_color(shape_info.line_color);
    fl_line_style(FL_SOLID | FL_JOIN_MITER, shape_info.line_width*(int)world_scale);
    fl_begin_loop();
    fl_vertex(normalsx, normalsy); fl_vertex(normalex, normalsy);
    fl_vertex(normalex, normaley); fl_vertex(normalsx, normaley);
    fl_vertex(normalsx, normalsy);
    fl_end_loop();
}

void Rect::draw_bbox()
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

bool Rect::is_inside_bbox(Vector2f v)
{
    update_bbox();
    //printf("(%f, %f) - (%f, %f - %f, %f)\n", v.x, v.y, bboxs.x, bboxs.y, bboxe.x, bboxe.y);
    if (((v.x > bboxs.x) && (v.x < bboxe.x))
        && ((v.y > bboxs.y) && (v.y < bboxe.y))) {
        //printf("is inside\n");
        return true;
    }
    //printf("is outside\n");
    return false;
}

Circle::Circle()
{
    max_nodes = 2;
    nodes.reserve(max_nodes);
}

void Circle::update_bbox()
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

void Circle::draw_shape()
{
    int sx, sy, ex, ey;
    world_to_scr(nodes[0].pos, sx, sy);
    world_to_scr(nodes[1].pos, ex, ey);
    float r = sqrtf((float)pow(sx-ex, 2) + (float)pow(sy-ey, 2));

    // NOTE(daniel): FLTK seems to have a bug with color assigning stores fill
    // color state bleeds to the fl_circle() shape that has no fill. The work
    // around is pretty simple use loop to do the line.

    fl_color(shape_info.fill_color);
    fl_begin_polygon();
    fl_arc((float)sx, (float)sy, r, 0, 360);
    fl_end_polygon();

    fl_color(shape_info.line_color);
    fl_line_style(FL_SOLID | FL_JOIN_MITER, shape_info.line_width*(int)world_scale);
    fl_begin_loop();
    fl_arc((float)sx, (float)sy, r, 0, 360);
    fl_end_loop();
}

bool Circle::is_inside_bbox(Vector2f v)
{
    update_bbox();
    //printf("(%f, %f) - (%f, %f - %f, %f)\n", v.x, v.y, bboxs.x, bboxs.y, bboxe.x, bboxe.y);
    if (((v.x > bboxs.x) && (v.x < bboxe.x))
        && ((v.y > bboxs.y) && (v.y < bboxe.y))) {
        //printf("is inside\n");
        return true;
    }
    //printf("is outside\n");
    return false;
}

void Circle::draw_bbox()
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

Node* Poly::get_next_node(const Vector2f p)
{
    Node n;
    n.parent = this;
    n.pos = p;
    nodes.push_back(n);
    return &nodes[nodes.size() - 1];
}

void Poly::draw_shape()
{
    int px, py;

    fl_color(shape_info.fill_color);
    fl_begin_polygon();
    for (size_t i = 0; i < nodes.size(); ++i) {
        world_to_scr(nodes[i].pos, px, py);
        fl_vertex(px, py);
    }
    fl_end_polygon();

    fl_color(shape_info.line_color);
    fl_line_style(FL_SOLID | FL_JOIN_MITER, shape_info.line_width*(int)world_scale);
    fl_begin_line();
    for (size_t i = 0; i < nodes.size(); ++i) {
        world_to_scr(nodes[i].pos, px, py);
        fl_vertex(px, py);
    }
    fl_end_line();
}

void Poly::update_bbox()
{
    // TODO(daniel)
}

void Poly::draw_bbox()
{
    //int sx, sy, ex, ey;
}

bool Poly::is_inside_bbox(Vector2f v)
{
    return false;
}


BezierHandle* Bezier::get_next_handle(const BezierHandle bh)
{
    bhandles.push_back(bh);
    return &bhandles[bhandles.size() - 1];
}

void Bezier::draw_shape()
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

void Bezier::draw_bbox()
{
}
void Bezier::update_bbox()
{
}
bool Bezier::is_inside_bbox(Vector2f v)
{
    return false;
}

