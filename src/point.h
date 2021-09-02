#pragma once

#include <ostream>

class Point {
public:
	Point(double x, double y) : m_x{ x }, m_y{ y } { };
	double get_x() const { return m_x; }
	void set_x(double x) { m_x = x; }
	double get_y() const { return m_y; }
	void set_y(double y) { m_y = y; }
private:
	double m_x, m_y;
};

std::ostream& operator<<(std::ostream& os, const Point& p)
{
	return os << '(' << p.get_x()
		<< ", " << p.get_y() << ')';
}

struct Vector { double x, y; };

std::ostream& operator<<(std::ostream& os, const Vector& v)
{
	return os << '[' << v.x
		<< ", " << v.y << ']';
}