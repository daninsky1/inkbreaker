#pragma once

#include <vector>
#include <ostream>

#include <ostream>


class Point {
public:
	Point(double x, double y);
	double get_x() const;
	void set_x(double x);
	double get_y() const;
	void set_y(double y);
private:
	double m_x, m_y;
};

struct Vector { double x, y; };

//enum class obj_type {
//	POLYGON,
//	POLYMESH,
//	CURVE,
//	PATH,
//	TEXT
//};
//
//class Object {
//public:
//	Object(Point o)
//		: m_origin{ o },
//		m_scale{ 1.0, 1.0 },
//		m_rotation{ 0 },
//		m_type{ obj_type::POLYGON }
//	{ };
//	Point get_origin() const { return m_origin; };
//	void set_origin(Point o) { m_origin = o; };
//
//	Point get_scale() const { return m_scale; };
//	void set_scale(Point s) { m_scale = s; };
//
//
//	double get_rotation() const { return m_rotation; };
//	void set_rotation(double radians) { m_rotation = radians; };
//
//private:
//	Point m_origin;
//	Point m_scale;
//	double m_rotation;	// radians
//	obj_type m_type;
//};
//
//class Line {
//public:
//	Line(Point a, Point b) : m_a{a}, m_b{b} { }
//	Point get_a() const { return m_a; }
//	void set_a(Point p) { m_a = p; }
//	Point get_b() const { return m_b; }
//	void set_b(Point p) { m_b = p; }
//private:
//	Point m_a;
//	Point m_b;
//};