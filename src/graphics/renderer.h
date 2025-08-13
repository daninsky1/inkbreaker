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

    // RENDER TARGET
    virtual void bindRenderTarget(std::shared_ptr<Surface> surface) = 0;

    virtual void releaseRenderTarget() = 0;

    // DRAWING OPERATIONS
    virtual void clear(const Color& color) = 0;

    virtual void drawRect(const Rect& rectangle, const Paint& paint) = 0;

    // STATE
    virtual void restore() = 0;

    virtual void save() = 0;

    // TRANSFORMATIONS
    virtual void translate(int32_t x, int32_t y) = 0;

    virtual void translate(IPoint pos) = 0;

    // CLIPPING
    virtual void clipRect(IRect rect) = 0;

    // FACTORIES
    static Renderer* create();

    static RendererAPI getRendererAPI() { return _rendererAPI; };
private:
    static RendererAPI _rendererAPI;
};

} // gfx
