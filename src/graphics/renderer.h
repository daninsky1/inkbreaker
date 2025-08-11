//
// Created by daniel on 09/08/2025.
//

#pragma once

#include <memory>

#include <blend2d.h>

#include "blend2d/blend2d_surface.h"
#include "paint.h"

namespace gfx {

enum class RendererAPI
{
    NONE,
    BLEND_2D
};

class Renderer
{
public:

    virtual ~Renderer() = default;

    virtual void bindRenderTarget(std::shared_ptr<Surface> surface) = 0;

    virtual void releaseRenderTarget() = 0;

    virtual void clear(const Color& color) = 0;

    virtual void drawRectangle(const Rectangle& rectangle, const Paint& paint) = 0;

    static Renderer* create();

    static RendererAPI getRendererAPI() { return _rendererAPI; };
private:
    static RendererAPI _rendererAPI;
};

} // gfx
