//
// Created by daniel on 10/08/2025.
//

#include "../blend2d/blend2d_renderer.h"

#include <iostream>

namespace gfx {

Blend2DRenderer::Blend2DRenderer()
{
}

Blend2DRenderer::~Blend2DRenderer()
{
    if (_surface == nullptr) return;
}

void Blend2DRenderer::bindRenderTarget(std::shared_ptr<Surface> surface)
{
    switch (Renderer::getRendererAPI()) {
    case RendererAPI::BLEND_2D: {
        _surface = std::static_pointer_cast<Blend2dSurface>(surface);
        break;
    }
    default:
        assert(false);  // NOT SUPPORTED
        _surface = nullptr;
    }
    _blContext.begin(_surface->getBLImage());
}

void Blend2DRenderer::releaseRenderTarget()
{
    _surface = nullptr;
    _blContext.end();
}

void Blend2DRenderer::clear(const Color& color)
{
    if (_surface == nullptr) return;
    auto blImage = _surface->getBLImage();
    _blContext.fillRect(
        BLRectI(0, 0, blImage.width(), blImage.height()),
        BLRgba32(color.pack32())
    );
}

void Blend2DRenderer::drawRect(const Rect& rectangle, const Paint& paint)
{
    if (_surface == nullptr) {
        assert(false);
    }
    _blContext.fillRect(
        BLRectI(rectangle.x, rectangle.y, rectangle.width, rectangle.height),
        BLRgba32(paint.getColor().pack32())
    );
}

void Blend2DRenderer::save()
{
    _blContext.save();
}

void Blend2DRenderer::restore()
{
    _blContext.restore();
}

void Blend2DRenderer::translate(int32_t x, int32_t y)
{
    _blContext.translate(x, y);
}

void Blend2DRenderer::translate(IPoint pos)
{
    _blContext.translate(pos.x, pos.x);
}

void Blend2DRenderer::clipRect(IRect rect)
{
    _blContext.clipToRect(BLRectI{rect.x, rect.y, rect.width, rect.height});
}

} // gfx

