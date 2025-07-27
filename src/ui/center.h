//
// Created by daniel on 26/07/2025.
//
#pragma once

#include <include/core/SkColor.h>

#include "layout.h"
#include "widget.h"

namespace ui {

class Center : public SingleChildWidget {
public:
    Center();
    Size layout(const BoxConstraint& constraint) override;
    void setColor(SkColor4f color) {
        _backgroundColor = color;
    }

    void render(SkCanvas*canvas,uint32_t offsetX,uint32_t offsetY) override;
protected:
    SkColor4f _backgroundColor = SkColors::kWhite;
};

} // ui
