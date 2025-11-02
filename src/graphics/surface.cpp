//
// Created by daniel on 10/08/2025.
//

#include "surface.h"

#include "renderer.h"
#include "blend2d/blend2d_renderer.h"
#include "blend2d/blend2d_surface.h"

namespace gfx {

std::shared_ptr<Surface> Surface::create(ImageInfo textureInfo)
{
    switch (Renderer::getRendererAPI()) {
    case RendererAPI::BLEND_2D: {
        auto blend2dSurface = std::make_shared<Blend2DSurface>(textureInfo);
        return blend2dSurface;
        break;
    }
    default:
        // NOT SUPPORTED
        assert(false);
        return nullptr;
    }
}

} // gfx