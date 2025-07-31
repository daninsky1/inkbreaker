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
    Size layout(const BoxConstraints& constraint) override;
    void setColor(SkColor4f color) {
        _color = color;
    }

    void render(SkCanvas*canvas, Position offset) override;
protected:
    SkColor4f _color = SkColors::kWhite;
};

} // ui
