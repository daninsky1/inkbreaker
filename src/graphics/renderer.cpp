//
// Created by daniel on 09/08/2025.
//

#include "renderer.h"

#include "blend2d/blend2d_renderer.h"

namespace gfx {

RendererAPI Renderer::_rendererAPI = RendererAPI::BLEND_2D;

Renderer* Renderer::create()
{
    switch (getRendererAPI()) {
    case RendererAPI::BLEND_2D:
        return new Blend2DRenderer();
    default:
        assert(false);  // API NOT SUPPORTED
        return nullptr;
    }
}

} // gfx
