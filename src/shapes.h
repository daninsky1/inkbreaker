#pragma once

#include <math.h>

#include <iostream>
#include <vector>
#include <ostream>
#include <FL/Fl.H>
#include <FL/fl_draw.H>

struct Pointi { int x, y; };
struct Vector { float x, y; };

struct ShapeInfo {
    // TODO(daniel): Add join method after selection is possible
    int line_width;     // In pixels
    Fl_Color line_color;
    Fl_Color fill_color;
};

struct Shape;
struct Node {
	Shape* parent;
	Vector pos;
};

struct Shape {
	// TODO: REPLACE STD::VECTOR WITH STD::ARRAY
	static double world_scale;
	static Vector world_offset;

    ShapeInfo sinfo{ 1, FL_WHITE, FL_BLACK };

	std::vector<Node> nodes;
	uint32_t max_nodes = 0;

    // NOTE(daniel): Bounding box start point and end point
    Vector bboxs{ 0.0f, 0.0f };
    Vector bboxe{ 0.0f, 0.0f };

    virtual void update_bbox() { };

	virtual void draw_shape() = 0;
    virtual void draw_bbox() { };
    virtual bool is_inside_bbox(Vector &r) { return false; };

    virtual void translate() { };
    virtual void rotate() { };
    virtual void scale() { };

    virtual std::string type() { return std::string{"Shape"}; };
	Node* get_next_node(const Vector& p);
	void draw_nodes();
	void world_to_scr(Vector& v, int& scrx, int& scry);

    void set_shape_info(ShapeInfo si) { sinfo = si; };
};

struct SelectBox : public Shape {
    // NOTE(daniel): SelectBox occur in world space like any other shape and
    // shown in screen space but we may want 
    int scrw, scrh;
	SelectBox();
	void draw_shape() override;
};

struct Line : public Shape {
	Line();
	void draw_shape() override;
    void draw_bbox() override;
    void update_bbox() override;
    std::string type() override { return std::string{"line"}; };
};

struct Rect : public Shape {
	Rect();
    void update_bbox() override;
	void draw_shape() override;
    void draw_bbox() override;
    bool is_inside_bbox(Vector &v) override;
    std::string type() override { return std::string{"rect"}; };
};

struct Circle : public Shape {
    Circle();
    void draw_shape() override;
    void draw_bbox() override;
    void update_bbox() override;
    bool is_inside_bbox(Vector &v) override;
    std::string type() override { return std::string{"circle"}; }
};
