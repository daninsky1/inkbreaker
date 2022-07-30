#pragma once

#include <math.h>

#include <iostream>
#include <vector>
#include <ostream>
#include <FL/Fl.H>
#include <FL/fl_draw.H>

#include "../coordinates.h"


struct SceneObject {
    Vec2f origin{ 0.0f, 0.0f };      // Local origin
    float scale{ 1.0f };                // Local scale
};

struct Polygon : public SceneObject {
    Polygon();
    Fl_Color line_color;
    Fl_Color fill_color;
};

struct OpenLineSegment : public SceneObject {
    Fl_Color line_color;
};



struct ShapeInfo {
    // TODO(daniel): Add join method after selection is possible
    int line_width;     // In pixels
    Fl_Color line_color;
    Fl_Color fill_color;    // If Shape has no fill this is ignored
    bool show_line; 
    bool show_fill;
};

struct Shape;
struct Node {
    Shape* parent;
    Vec2f pos;
};

struct Shape {
	// TODO: REPLACE STD::VECTOR WITH STD::ARRAY
    static float world_scale;
    static Vec2f world_offset;

    //ShapeInfo shape_info{ 1, FL_WHITE, FL_BLACK, true, true };
    ShapeInfo shape_info{ 1, FL_WHITE, FL_BLACK, true, true };

    std::vector<Node> nodes;
    uint32_t max_nodes = 0;

    // NOTE(daniel): Bounding box start point and end point
    Vec2f bboxs{ 0.0f, 0.0f };
    Vec2f bboxe{ 0.0f, 0.0f };

    virtual void update_bbox() { };

	virtual void draw_shape() = 0;
    virtual void draw_bbox() { };
    virtual bool is_inside_bbox(Vec2f r) { return false; };

    virtual void translate() { };
    virtual void rotate() { };
    virtual void scale() { };

    virtual std::string type() { return std::string{"Shape"}; };
	virtual Node* get_next_node(const Vec2f p);
	void draw_nodes();
	void world_to_scr(Vec2f v, int& scrx, int& scry);

    void set_shape_info(ShapeInfo si) { shape_info = si; };
};

struct BBox : public Shape {
    // NOTE(daniel): Bounding box, BBox occur in world space like any other
    // shape and shown in screen space but we may want 
    int scrw, scrh;
	BBox();
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
    bool is_inside_bbox(Vec2f v) override;
    std::string type() override { return std::string{"rect"}; };
};

struct Circle : public Shape {
    Circle();
    void draw_shape() override;
    void draw_bbox() override;
    void update_bbox() override;
    bool is_inside_bbox(Vec2f v) override;
    std::string type() override { return std::string{"circle"}; }
};

struct Poly : public Shape {
    Poly() { }
    Node* get_next_node(const Vec2f p) override;
    void draw_shape() override;
    void draw_bbox() override;
    void update_bbox() override;
    bool is_inside_bbox(Vec2f v) override;
    std::string type() override { return std::string{"polygon"}; }
};

struct BezierHandle {
    Vec2f point;
    Vec2f head;
    Vec2f tail;
};

struct Bezier : public Shape {
    // NOTE(daniel): Bezier points and handle:
    // px0, py0, px1, py1 are the curve points
    // hx0, hy0, hx1, hy1 are the curve handles
    // They are laid down in this order:
    // px0, py0, hx0, hy0, hx1, hy1, px1, py1
    // The h0 are the head curve handle and the h1 are the tail curve handle
    std::vector<BezierHandle> bhandles;
    Bezier() { }
    BezierHandle *get_next_handle(BezierHandle bh);
    void draw_shape() override;
    void draw_bbox() override;
    void update_bbox() override;
    bool is_inside_bbox(Vec2f v) override;
    std::string type() override { return std::string{"bezier"}; }
};


void translate(float offx, float offy, Shape *s);
