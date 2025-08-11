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
}

void Blend2DRenderer::drawRectangle(const Rectangle& rectangle, const Paint& paint)
{
    if (_surface == nullptr) {
        assert(false);
    }

    auto color = paint.getColor();
    uint32_t packed =
        (static_cast<uint32_t>(color.a) << 24) |
        (static_cast<uint32_t>(color.r) << 16) |
        (static_cast<uint32_t>(color.g) <<  8) |
        (static_cast<uint32_t>(color.b) <<  0);
    _blContext.fillRect(
        BLRectI(rectangle.x, rectangle.y, rectangle.width, rectangle.height),
        BLRgba32(packed)
    );
}

} // gfx

