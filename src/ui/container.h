#pragma once

#include "widget.h"

#include "style_properties.h"

namespace ui
{

/*
* Widget container class.
* This class is intended to hold common positioning and sizing of widgets.
*/
class Container : public SingleChildWidget
{
public:
    Container(uint64_t id)
        : SingleChildWidget(id), _margin(0.0), _padding(0.0) { }
    
    void setColor(SkColor4f color) { _backgroundColor = color; }
    SkColor4f getColor() const { return _backgroundColor; }
    
    void setMargin(const css::Margin& margin);
    const css::Margin& getMargin() const;
    void setPadding(const css::Padding& padding);
    const css::Padding& getPadding() const;
    
    void render(SkCanvas* canvas,  uint32_t offsetX = 0, uint32_t offsetY = 0) override;
private:
    css::Margin _margin;
    css::Padding _padding;
    SkColor4f _backgroundColor = SkColors::kGray;
};

}