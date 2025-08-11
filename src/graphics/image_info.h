//
// Created by daniel on 09/08/2025.
//

#pragma once

#include <cassert>

#include "geometry.h"
#include "color.h"

namespace gfx {

constexpr size_t bytesPerPixel(ColorType colorType) noexcept
{
    switch (colorType) {
    case ColorType::PREMUL_RGBA_8888:
    case ColorType::PREMUL_ARGB_8888:
        return 4;
    default:
        assert(false);
        return 0;
    }
}

constexpr size_t alphaChannelIndex(ColorType colorType) noexcept
{
    switch (colorType) {
    case ColorType::PREMUL_RGBA_8888: return 3;
    case ColorType::PREMUL_ARGB_8888: return 0;
    default: assert(false); return -1;
    }
}

// class Texture;
struct ImageInfo
{
    Size dimension{0, 0};
    ColorType colorType = ColorType::DEFAULT;
    [[nodiscard]] bool isEmpty() const { return dimension.isEmpty(); }

    size_t byteCount() const
    {
        size_t pixelCount = static_cast<size_t>(dimension.width) *
               static_cast<size_t>(dimension.height);
        switch (colorType) {
        case ColorType::PREMUL_RGBA_8888:
        case ColorType::PREMUL_ARGB_8888: {
            return pixelCount * bytesPerPixel(colorType);
        }
        default: {
            assert(false);
            return 0;
        }
        }
    }

    size_t getStride()const
    {
        size_t pixelRowCount = static_cast<size_t>(dimension.width);
        switch (colorType) {
        case ColorType::PREMUL_RGBA_8888:
        case ColorType::PREMUL_ARGB_8888: {
            return pixelRowCount * bytesPerPixel(colorType);
        }
        default: {
            assert(false);
            return 0;
        }
        }
    }

    size_t getBytesPerPixel() const
    {
        return bytesPerPixel(colorType);
    }

    size_t getAlphaChannelIndex()
    {
        return alphaChannelIndex(colorType);
    }
};

} // gfx
