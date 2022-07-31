#pragma once

#include "../coordinates.h"


struct Shape {
    static float world_scale;
    static Vec2f world_offset;

    // NOTE(daniel): Top left and bottom right of the bounding box
    Vec2f bboxs{ 0.0f, 0.0f };
    Vec2f bboxe{ 0.0f, 0.0f };

    virtual void update_bbox() { };

	virtual void draw() = 0;
    virtual bool is_inside_bbox(Vec2f point) { return false; };

    virtual std::string type() { return std::string{"Shape"}; };

	void world_to_scr(Vec2f v, int& scrx, int& scry);
    
    virtual void translate() { };
    virtual void rotate() { };
    virtual void scale() { };
    
    virtual ~Shape() { };
    //void set_shape_info(ShapeInfo si) { shape_info = si; };
};
