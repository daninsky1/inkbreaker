//
// Created by daniel on 16/08/2025.
//

#pragma once
#include "graphics/font.h"

#include <memory>
#include <blend2d.h>

namespace gfx {

class Blend2DTypeface final : public Typeface
{
public:
    Blend2DTypeface() = default;
    ~Blend2DTypeface() override = default;
    BLResult createFromFile(std::string filepath);
    BLFontFace getBLFontFace() const;
private:
    BLFontFace _typeface;
};

class Blend2DFont final : public Font
{
public:
    Blend2DFont(BLFontFace typeface, float size)
    {
        _font.createFromFace(typeface, size);
    }
    
    BLFont getBLFont() const { return _font; }
private:
    BLFont _font;
};

} // gfx
