//
// Created by daniel on 10/08/2025.
//

#pragma once

#include "../renderer.h"
#include "blend2d.h"

namespace gfx {

/**
 * Blend2D renderer
 */
class Blend2DRenderer final : public Renderer
{
public:
    explicit Blend2DRenderer();
    ~Blend2DRenderer() override;

    void bindRenderTarget(std::shared_ptr<Surface> surface) override;
    void releaseRenderTarget() override;
    void clear(const Color& color) override;
    void drawRect(const Rect& rectangle, const Paint& paint) override;
    void save() override;
    void restore() override;
    void translate(int32_t x, int32_t y) override;
    void translate(IPoint pos) override;
    void clipRect(IRect rect) override;
private:
    std::shared_ptr<Blend2dSurface> _surface{};
    BLContext _blContext{};
};

} // gfx
