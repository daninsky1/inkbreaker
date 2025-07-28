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
    Size layout(const BoxConstraint& constraint) override;
    void render(SkCanvas*canvas,uint32_t offsetX,uint32_t offsetY) override;

    void setColor(SkColor4f color) { _color = color; }
    void setPadding(BoxSpace padding) { _padding = padding; }
protected:
    BoxSpace _padding;
    SkColor4f _color = SkColors::kWhite;
};
} // ui
