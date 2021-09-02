#include "shapes.h"


Point::Point(double x, double y) : m_x{ x }, m_y{ y } { };
double Point::get_x() const { return m_x; }
void Point::set_x(double x) { m_x = x; }
double Point::get_y() const { return m_y; }
void Point::set_y(double y) { m_y = y; }

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
		fl_color(FL_RED);
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
	fl_line_style(FL_SOLID, 1*world_scale);
	fl_line(sx, sy, ex, ey);
}
