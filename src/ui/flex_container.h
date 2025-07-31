#pragma once

#include <include/core/SkPaint.h>
#include <include/effects/SkDiscretePathEffect.h>

#include "widget.h"

#include "layout.h"

namespace ui
{

/*
* Widget FlexContainer.
* This Widget is intended to hold common some flexbox features.
*/
class FlexContainer : public MultiChildWidget
{
public:
    FlexContainer()
        : _margin(0.0), _padding(0.0) { }
    ~FlexContainer() = default;
    
    void setColor(SkColor4f color) { _backgroundColor = color; }
    SkColor4f getColor() const { return _backgroundColor; }
    void setMargin(const css::Margin& margin) { _margin = margin; }
    const css::Margin& getMargin() const { return _margin;}
    void setPadding(const css::Padding& padding) { _padding = padding; }
    const css::Padding& getPadding() const { return _padding; }
    
    void render(SkCanvas*canvas, Position offset) override;
private:
    SkColor4f _backgroundColor = SkColors::kGray;
    css::Margin _margin;
    css::Padding _padding;
    css::FlexDirection _flexDirection = css::FlexDirection::Row; // Default flex direction
};

}