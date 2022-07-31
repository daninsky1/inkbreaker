#include "polygon.h"


Vec2f *Polygon::add_point(const Vec2f p)
{
    points.push_back(p);
    return &points[points.size() - 1];
}

Vec2f *Polygon::insert_point(int index, const Vec2f p)
{
    vector<Vec2f>::iterator it = points.begin() + index;
    points.insert(it, p);
    return &points[index];
}
