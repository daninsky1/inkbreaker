//
// Created by daniel on 09/08/2025.
//

#pragma once

#include "color.h"

namespace gfx {

enum class Style
{
    FILL_STYLE,
    STROKE_STYLE,
    STROKE_AND_FILL_STYLE
};

/**
 * Controll Options for drawing
 */
class Paint {
public:
    Paint() = default;

    Color getColor() const
    {
        return _color;
    }

    Paint& setColor(Color color)
    {
        _color = color;
        return *this;
    }

    Paint& setStyle(Style style)
    {
        _style = style;
        return *this;
    }

    Paint& setAntiAlias(bool antiAlias)
    {
        _antiAlias = antiAlias;
        return *this;
    }

    static Paint create()
    {
        return Paint();
    }

private:
    Color _color;
    Style _style;
    bool _antiAlias = false;
};

} // gfx
