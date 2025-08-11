//
// Created by daniel on 09/08/2025.
//

#pragma once

#include "color.h"

namespace gfx {

/**
 * Controll Options for drawing
 */
class Paint {
public:
    Paint() = default;

    Paint& set(Color color)
    {
        _color = color;
        return *this;
    }

    Color getColor() const
    {
        return _color;
    }

private:
    Color _color;
};

} // gfx
