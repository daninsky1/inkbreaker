//
// Created by daniel on 02/08/2025.
//

#include "text.h"

#include "graphics/font.h"

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
    renderer->translate(offset.x, offset.y);
    renderer->clipRect(gfx::Rect{_size.width, _size.height});

    gfx::Paint paint = gfx::Paint::create()
        .setStyle(gfx::Style::FILL_STYLE)
        .setColor(_backgoundColor);
    renderer->drawRect(gfx::Rect{0, 0, _size.width, _size.height}, paint);

    std::shared_ptr<gfx::Typeface> typeface = gfx::Typeface::createFromFile(_fontFilepath);
    std::shared_ptr<gfx::Font> font = gfx::Font::createFromTypeface(typeface, 24.0f);

    

    // TODO(Daniel S): Renderizar texto


    renderer->restore();
}

} // ui