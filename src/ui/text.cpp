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

void Text::render(SkCanvas* canvas, Position offset)
{
    canvas->save();
    // canvas->translate(offset.x, offset.y);
    // canvas->clipRect(SkRect::MakeWH(_size.width, _size.height));
    // canvas->drawColor(_backgoundColor);

    // Restaura o estado anterior do canvas
    canvas->restore();
}

} // ui