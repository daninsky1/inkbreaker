#pragma once

#include <FL/Fl.H>

#include "../coordinates.h"


class Shape {
public:
    static float world_scale;
    static Vec2f world_offset;
    
    void world_to_scr(Vec2f v, int& scrx, int& scry) {
        scrx = static_cast<int>((v.x - world_offset.x) * world_scale);
        scry = static_cast<int>((v.y - world_offset.y) * world_scale);
    } 

    // NOTE(daniel): Top left and bottom right of the bounding box
    Vec2f bboxs{ 0.0f, 0.0f };
    Vec2f bboxe{ 0.0f, 0.0f };

    virtual void update_bbox() { };

	virtual void draw() = 0;
    virtual bool is_inside_bbox(Vec2f point) { return false; };

    std::string type() { return m_type; };

    virtual void translate() { };
    virtual void rotate() { };
    virtual void scale() { };
    
    virtual ~Shape() { };
private:
    std::string m_type = "shape";
};

struct ShapeInfo {
    // TODO(daniel): Add join method after selection is possible
    int line_width;     // In pixels
    Fl_Color line_color;
    Fl_Color fill_color;    // If Shape has no fill this is ignored
    bool show_line; 
    bool show_fill;
};
