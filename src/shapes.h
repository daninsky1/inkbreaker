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

struct sShape;
struct sNode {
	sShape* parent;
	Vector pos;
};

struct sShape {
	// TODO: REPLACE STD::VECTOR WITH STD::ARRAY
	static double world_scale;
	static Vector world_offset;

    ShapeInfo sinfo{ 1, FL_WHITE, FL_BLACK };

	std::vector<sNode> nodes;
	uint32_t max_nodes = 0;

    // NOTE(daniel): Bounding box start point and end point
    Vector bboxs{ 0.0f, 0.0f };
    Vector bboxe{ 0.0f, 0.0f };

    virtual void update_bbox() { };

	virtual void draw_shape() = 0;
    virtual void draw_bbox() { };

    virtual std::string type() { return std::string{"sShape"}; };
	sNode* get_next_node(const Vector& p);
	void draw_nodes();
	void world_to_scr(Vector& v, int& scrx, int& scry);

    void set_shape_info(ShapeInfo si) { sinfo = si; };
};

struct sSelectBox : public sShape {
	sSelectBox();
	void draw_shape() override;
};

struct sLine : public sShape {
	sLine();
	void draw_shape() override;
    void draw_bbox() override;
    void update_bbox() override;
    std::string type() override { return std::string{"line"}; };
};

struct sRect : public sShape {
	sRect();
	void draw_shape() override;
    void draw_bbox() override;
    void update_bbox() override;
    std::string type() override { return std::string{"rect"}; };
};

struct sCircle : public sShape {
    sCircle();
    void draw_shape() override;
    void draw_bbox() override;
    void update_bbox() override;
    std::string type() override { return std::string{"circle"}; }
};
