//
// Created by daniel on 02/08/2025.
//

#pragma once

#include <cassert>

#include <include/core/SkColor.h>

#include "widget.h"

namespace ui {

class Text : public LeafWidget {
public:
    Text(std::string value);

    Size layout(const BoxConstraints& boxConstraints) override;
    void render(gfx::Renderer* renderer, Position offset) override;
private:
    void skiaTextRender(gfx::Renderer* renderer, Position offset);
    std::string _value;
    gfx::Color _color = gfx::Colors::WHITE;
    gfx::Color _backgoundColor = gfx::Colors::TRANSPARENT;
    int32_t _fontSize = 24;
    std::string _fontFilepath = "resources/fonts/roboto/static/Roboto-Regular.ttf";
};

} // ui
