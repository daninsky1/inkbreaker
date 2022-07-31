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
    void draw() override { };
    void update_bbox() override { };
    bool is_inside_bbox(Vec2f v) override{ return false; };
    std::string type() override { return std::string{"polygon"}; }
    
    Vec2f *add_point(const Vec2f point);
    Vec2f *insert_point(int index, const Vec2f point);
private:
    vector<Vec2f> points;
};
