#include "container.h"


namespace ui
{
void Container::setMargin(const css::Margin& margin)
{
    _margin = margin;
}
const css::Margin& Container::getMargin() const
{
    return _margin;
}
void Container::setPadding(const css::Padding& padding)
{
    _padding = padding;
}
const css::Padding& Container::getPadding() const
{
    return _padding;
}

void Container::render(SkCanvas* canvas,  uint32_t offsetX, uint32_t offsetY)
{
    SDL_Log("Rendering widget at (%d, %d) with size (%d, %d)", _x + offsetX, _y + offsetY, _width, _height);
    
    canvas->save();
    canvas->clipRect(SkRect::MakeWH(256, 128));
    canvas->clear(SkColorSetARGB(0x80, 0xFF, 0x00, 0x00));
    canvas->restore();
    canvas->save();
    canvas->clipRect(SkRect::MakeWH(150, 192));
    canvas->clear(SkColorSetARGB(0x80, 0x00, 0xFF, 0x00));
    canvas->restore();
    canvas->clipRect(SkRect::MakeWH(75, 256));
    canvas->clear(SkColorSetARGB(0x80, 0x00, 0x00, 0xFF));
}
} // namespace ui