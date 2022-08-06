#pragma once

#include <vector>

#include "shape.h"

using namespace std;
// TODO(daniel): Make this private and add new attributes to store, location,
// scale and rotation, using setter to update those attributes using the points
// or location, scale and rotation.
class BezierHandle {
public:
    Vec2f point;
    Vec2f head;
    Vec2f tail;
};

class Bezier : public Shape {
public:
    Bezier() { };
    ~Bezier() { };
    ShapeInfo shape_info{ 1, FL_BLACK, FL_WHITE, true, true };
    void draw() override;
    void update_bbox() override { };
    bool is_inside_bbox(Vec2f v) override { return false; };
    
    BezierHandle *add_bhandle(const BezierHandle point);
    BezierHandle *add_bhandle(Vec2f point, Vec2f head, Vec2f tail);
    BezierHandle *insert_bhandle(int index, const BezierHandle point);
    BezierHandle *last_bhandle() { return &bhandles[bhandles.size() - 1]; }
    
    bool empty() { return bhandles.empty(); }
    size_t size() { return bhandles.size(); }
    void pop_back() { return bhandles.pop_back(); }
    
    void set_shape_info(ShapeInfo si) { shape_info = si; };
    vector<BezierHandle> bhandles;
};
