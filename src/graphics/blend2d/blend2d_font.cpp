//
// Created by daniel on 16/08/2025.
//

#include "blend2d_font.h"

namespace gfx {

BLResult Blend2DTypeface::createFromFile(std::string filepath)
{
    return _typeface.createFromFile(filepath.c_str());
}

BLFontFace Blend2DTypeface::getBLFontFace() const
{
    return _typeface;
}


} // gfx