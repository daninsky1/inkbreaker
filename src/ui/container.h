#pragma once

#include <format>

#include <include/core/SkPaint.h>
#include <include/effects/SkDiscretePathEffect.h>

#include "widget.h"

#include "layout.h"
#include "padding.h"

namespace ui
{

/*
* Widget container class.
* This class is intended to hold common positioning and sizing of widgets.
*/
class Container : public SingleChildWidget
{
public:
    Container()
        : _margin(0.0), _padding(0, 0, 0, 0) { }
    
    void setColor(SkColor4f color) { _backgroundColor = color; }
    SkColor4f getColor() const { return _backgroundColor; }
    void setMargin(const css::Margin& margin);
    const css::Margin& getMargin() const;
    void setPadding(const BoxSpace padding);
    BoxSpace getPadding() const;

    Size layout(const BoxConstraints& constraint) override;
    void render(SkCanvas*canvas, Position offset) override;
private:
    css::Margin _margin;
    BoxSpace _padding;
    SkColor4f _backgroundColor = SkColors::kGray;
};

}
