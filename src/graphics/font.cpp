//
// Created by daniel on 16/08/2025.
//

#include "graphics/renderer.h"
#include "graphics/font.h"

#include "blend2d/blend2d_font.h"

namespace gfx {

std::shared_ptr<Typeface> Typeface::createFromFile(std::string filepath)
{
    switch (Renderer::getRendererAPI()) {
    case RendererAPI::BLEND_2D:{
        auto blend2DFont = std::make_shared<Blend2DTypeface>();
        blend2DFont->createFromFile(filepath);
        return blend2DFont;
    }
    default:
        assert(false);  // API NOT SUPPORTED
        return nullptr;
    }
}

std::shared_ptr<Font> Font::createFromTypeface(std::shared_ptr<Typeface> typeface, float size)
{
    switch (Renderer::getRendererAPI()) {
    case RendererAPI::BLEND_2D:{
        auto blend2DTypeface = std::static_pointer_cast<Blend2DTypeface>(typeface);
        return std::make_shared<Blend2DFont>(blend2DTypeface->getBLFontFace(), size);
    }
    default:
        assert(false);  // API NOT SUPPORTED
        return nullptr;
    }
}

std::shared_ptr<Font>

}