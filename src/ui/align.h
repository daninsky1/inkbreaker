//
// Created by daniel on 27/07/2025.
//
#pragma once

#include "widget.h"

namespace ui
{

enum class Alignment
{
    TopLeft,
    TopCenter,
    TopRight,
    CenterLeft,
    Center,
    CenterRight,
    BottomLeft,
    BottomCenter,
    BottomRight
};

class Align : public SingleChildWidget
{
public:
    Align();
    Size layout(const BoxConstraints& constraint) override;
    void render(SkCanvas*canvas, Position offset) override;

    void setColor(SkColor4f color) { _color = color; }
    void setAlignment(const Alignment alignment) { _alignment = alignment; }
protected:
    SkColor4f _color = SkColors::kWhite;
    Alignment _alignment = Alignment::Center;
};

}   // namespace ui
