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
void Container::setPadding(const BoxSpace padding)
{
    _padding = padding;
}
BoxSpace Container::getPadding() const
{
    return _padding;
}

Size Container::layout(const BoxConstraints& constraint)
{
    if (_child != nullptr) {
        // Assumes the size of the child
        _size = _child->layout(constraint);
    } else if (!_size.hasSize()) {
        // Assumes the max size of the parent, this can lead to a bug, if later
        // be decided to implements the unrestricted widget, there will be no
        // reasable max size to get from constraint
        _size = {
            std::max(constraint.minWidth, constraint.maxWidth),
            std::max(constraint.minWidth, constraint.maxHeight)
        };
    }
    // Implementação de padding dentro do container
    _size.width += (_padding.left + _padding.right);
    _size.height += (_padding.top + _padding.bottom);

    return normalize(constraint);
}

void Container::render(gfx::Renderer* renderer, Position offset)
{
    renderer->save();

    renderer->translate(offset.x, offset.y);

    // Define o retângulo de clipping do container
    renderer->clipRect(gfx::Rect{0, 0, _size.width, _size.height});

    gfx::Paint paint;
    paint.setStyle(gfx::Style::FILL_STYLE);
    paint.setColor(_backgroundColor);

    renderer->drawRect(gfx::Rect{0, 0, _size.width, _size.height}, paint);

    // Restaura o estado anterior do renderer
    renderer->restore();

    if (_child != nullptr) {
        _child->render(renderer, offset.add(_padding.left, _padding.top));
    }
}
} // namespace ui
