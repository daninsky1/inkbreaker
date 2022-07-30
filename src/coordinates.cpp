#include "coordinates.h"


std::ostream& operator<<(std::ostream& os, const Vec2f& v)
{
    return os << '[' << v.x << ", " << v.y << ']';
}