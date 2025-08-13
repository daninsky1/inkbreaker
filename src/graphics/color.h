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

namespace Colors {
constexpr Color TRANSPARENT = {  0,   0,   0,   0};
constexpr Color BLACK       = {  0,   0,   0, 255};
constexpr Color DKGRAY      = { 64,  64,  64, 255};
constexpr Color GRAY        = {128, 128, 128, 255};
constexpr Color LTGRAY      = {191, 191, 191, 255};
constexpr Color WHITE       = {255, 255, 255, 255};
constexpr Color RED         = {255,   0,   0, 255};
constexpr Color GREEN       = {  0, 255,   0, 255};
constexpr Color BLUE        = {  0,   0, 255, 255};
constexpr Color YELLOW      = {255, 255,   0, 255};
constexpr Color CYAN        = {  0, 255, 255, 255};
constexpr Color MAGENTA     = {255,   0, 255, 255};
}

}
