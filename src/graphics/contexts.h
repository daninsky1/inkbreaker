//
// Created by daniel on 10/08/2025.
//

#pragma once

#include "blend2d/blend2d_renderer.h"

namespace gfx {
class RenderFactory
{
public:
    static Renderer* createBlend2DRender(ImageInfo info)
    {
        return new Blend2DRenderer(info);
    }
};
}
