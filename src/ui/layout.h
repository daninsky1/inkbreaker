#pragma once

#include <algorithm>
#include <cstdint>
/*
* Tries to mimic very porly css properties.
* This is not a real css implementation, just a way to store some style
* properties, inspired by css.
*/

namespace ui
{
struct BoxSpace
{
    int32_t top = 0;
    int32_t left = 0;
    int32_t bottom = 0;
    int32_t right = 0;
};

}

namespace css {
struct Margin {
    double top;
    double right;
    double bottom;
    double left;
    
    explicit Margin(double all)
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

struct Position
{
    int32_t x = 0, y = 0;

    [[nodiscard]] Position add(int32_t ox, int32_t oy) const {
        return {.x = x + ox, .y = y + oy};
    }
    [[nodiscard]] Position add(Position o) const {
        return {.x = x + o.x, .y = y + o.y};
    }
};

struct BoxConstraints;
struct Size
{
    int32_t width = 0, height = 0;

    [[nodiscard]] bool hasSize() const {
        return width > 0 && height > 0;
    }
};

struct Box {
    int32_t x = 0;
    int32_t y = 0;
    int32_t width = 0;
    int32_t height = 0;
};

struct BoxConstraints {
    int32_t minWidth = 0;
    int32_t minHeight = 0;
    int32_t maxWidth = 0;
    int32_t maxHeight = 0;

    Size clamp(Size& size) const {
        return {
            .width = std::clamp(size.width, minWidth, std::max(minWidth, maxWidth)),
            .height = std::clamp(size.height, minHeight, std::max(minHeight, maxHeight))
        };
    }

    [[nodiscard]] BoxConstraints tighten(const BoxConstraints& other) const {
        return {
            .minWidth = std::max(other.minWidth, minWidth),
            .minHeight = std::max(other.minHeight, minHeight),
            .maxWidth = std::min(other.maxWidth, maxWidth),
            .maxHeight = std::min(other.maxHeight, maxHeight)
        };
    }
};
