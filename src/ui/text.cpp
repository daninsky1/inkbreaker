//
// Created by daniel on 02/08/2025.
//

#include "text.h"

namespace ui {

Text::Text(std::string value)
    : _value(std::move(value))
{

}

Size Text::layout(const BoxConstraints& boxConstraints)
{
    return _size;
}

void Text::render(gfx::Renderer* renderer, Position offset)
{
    renderer->save();
    // renderer->translate(offset.x, offset.y);
    // renderer->clipRect(gfx::Rect{_size.width, _size.height));
    // renderer->drawColor(_backgoundColor);


    renderer->restore();
}

} // ui