#pragma once

#include <vector>

#include "shape.h"


/* Polygon may be an imprecise term, because this can draw only contours when
* the Polygon fill is transparent, open lines, one line(one-dimention shape)
* and a shapes with lines crossing.
* This is a lines, shapes, and poligons
*/
using namespace std;

class Polygon : public Shape {
public:
    Polygon() { };
    ~Polygon() { };
    ShapeInfo shape_info{ 1, FL_BLACK, FL_WHITE, true, true };
    void draw() override { };
    void update_bbox() override { };
    bool is_inside_bbox(Vec2f v) override { return false; };
    
    Vec2f *add_point(const Vec2f point);
    Vec2f *insert_point(int index, const Vec2f point);
    Vec2f *last_point() { return &points[points.size() - 1]; }
    
    bool empty() { return points.empty(); }
    size_t size() { return points.size(); }
    void pop_back() { return points.pop_back(); }
    
    void set_shape_info(ShapeInfo si) { shape_info = si; };
private:
    vector<Vec2f> points;
};
