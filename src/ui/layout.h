#pragma once

#include <cstdint>
/*
* Tries to mimic very porly css properties.
* This is not a real css implementation, just a way to store some style
* properties, inspired by css.
*/
namespace css {
struct Margin {
    double top;
    double right;
    double bottom;
    double left;
    
    Margin(double all)
        : top(all), right(all), bottom(all), left(all) {}
    Margin(double tb, double lr)
        : top(tb), right(lr), bottom(tb), left(lr) {}
    Margin(double t, double r, double b, double l)
        : top(t), right(r), bottom(b), left(l) {}
};

struct Padding {
    double top;
    double right;
    double bottom;
    double left;
    Padding(double all)
        : top(all), right(all), bottom(all), left(all) {}
    Padding(double tb, double lr)
        : top(tb), right(lr), bottom(tb), left(lr) {}
    Padding(double t, double r, double b, double l)
        : top(t), right(r), bottom(b), left(l) {}
};

enum class FlexDirection {
    Row,
    Column,
    RowReverse,
    ColumnReverse
};

}

struct Position { uint32_t x = 0, y = 0; };
struct Size
{
    uint32_t width = 0, height = 0;

    bool hasSize() const {
        return width > 0 && height > 0;
    }
};
struct Box {
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t width = 0;
    uint32_t height = 0;
};
struct BoxConstraint {
    uint32_t minWidth = 0;
    uint32_t minHeight = 0;
    uint32_t maxWidth = 0;
    uint32_t maxHeight = 0;
};
