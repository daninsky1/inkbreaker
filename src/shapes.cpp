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
