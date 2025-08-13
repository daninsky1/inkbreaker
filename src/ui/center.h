//
// Created by daniel on 26/07/2025.
//
#pragma once

#include "widget.h"

#include "layout.h"

namespace ui {

class Center : public SingleChildWidget {
public:
    Center();
    Size layout(const BoxConstraints& constraint) override;
    void setColor(gfx::Color color) {
        _color = color;
    }

    void render(gfx::Renderer* renderer, Position offset) override;
protected:
    gfx::Color _color = gfx::Colors::WHITE;
};

} // ui
