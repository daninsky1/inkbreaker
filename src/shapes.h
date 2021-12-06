#pragma once


#include <iostream>
#include <vector>
#include <ostream>
#include <FL/Fl.H>
#include <FL/fl_draw.H>

struct Pointi { int x, y; };

struct Vector { float x, y; };

struct sShape;
struct sNode {
	sShape* parent;
	Vector pos;
};

struct sShape {
	// TODO: REPLACE STD::VECTOR WITH STD::ARRAY
	std::vector<sNode> nodes;
	uint32_t max_nodes = 0;

	static double world_scale;
	static Vector world_offset;

	virtual void draw_shape() = 0;
    virtual std::string type() = 0;
	sNode* get_next_node(const Vector& p);
	void draw_nodes();
	void world_to_scr(Vector& v, int& scrx, int& scry);
};

struct sLine : public sShape {
	sLine();
	void draw_shape() override;
    std::string type() override { return std::string{"line"}; };
};

struct sRect : public sShape {
	sRect();
	void draw_shape() override;
    std::string type() override { return std::string{"rect"}; };
};


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
