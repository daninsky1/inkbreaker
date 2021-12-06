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
    ShapeInfo sinfo{ 1, FL_WHITE, FL_BLACK };

	std::vector<sNode> nodes;
	uint32_t max_nodes = 0;

	static double world_scale;
	static Vector world_offset;

	virtual void draw_shape() = 0;
    virtual std::string type() = 0;
	sNode* get_next_node(const Vector& p);
	void draw_nodes();
	void world_to_scr(Vector& v, int& scrx, int& scry);

    void set_shape_info(ShapeInfo si) { sinfo = si; };
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


struct sCircle : public sShape {
    sCircle();
    void draw_shape() override;
    std::string type() override { return std::string{"circle"}; }
};
