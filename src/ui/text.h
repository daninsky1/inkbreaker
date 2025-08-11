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
    void render(SkCanvas* canvas, Position offset) override;
private:
    void skiaTextRender(SkCanvas* canvas, Position offset);
    std::string _value;
    SkColor4f _color = SkColors::kWhite;
    SkColor4f _backgoundColor = SkColors::kTransparent;
    int32_t _fontSize = 24;
};

} // ui
