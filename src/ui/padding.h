//
// Created by daniel on 27/07/2025.
//

#pragma once

#include "layout.h"
#include "widget.h"

#include <include/core/SkCanvas.h>
#include <include/core/SkColor.h>

namespace ui {

class Padding : public SingleChildWidget {
public:
    Padding();
    Padding(uint32_t top, uint32_t left, uint32_t bottom, uint32_t right);
    Size layout(const BoxConstraints& constraint) override;
    void render(gfx::Renderer* renderer, Position offset) override;

    void setColor(gfx::Color color) { _color = color; }
    void setPadding(BoxSpace padding) { _padding = padding; }
    BoxSpace getPadding() const { return _padding; }
protected:
    BoxSpace _padding = {0, 0, 0, 0};
    gfx::Color _color = gfx::Colors::WHITE;
};
} // ui
