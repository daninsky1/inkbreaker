//
// Created by daniel on 09/08/2025.
//

#pragma once

#include <cinttypes>

namespace gfx {

enum class ColorType
{
    PREMUL_RGBA_8888,
    PREMUL_ARGB_8888,
    XRGB_8888,           // 8-bit padding
    DEFAULT = PREMUL_ARGB_8888
};

struct Color
{
    uint8_t r, g, b, a;
};
}
