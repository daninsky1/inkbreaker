//
// Created by daniel on 26/07/2025.
//

#include "center.h"

namespace ui {

Center::Center()
{

}

Size Center::layout(const BoxConstraints& constraint)
{
    BoxConstraints centerConstraint = {
        .minWidth = 0,
        .minHeight = 0,
        .maxWidth = constraint.maxWidth,
        .maxHeight = constraint.maxHeight
    };
    if (_child != nullptr) {
        Size childSize = _child->layout(centerConstraint);
        _childPosition = {
            .x = (constraint.maxWidth - childSize.width) / 2,
            .y = (constraint.maxHeight - childSize.height) / 2
        };
    }
    // Normalize Center size
    return normalize(constraint);
}

void Center::render(gfx::Renderer* renderer, Position offset)
{
    renderer->save();

    renderer->translate(offset.x, offset.x);

    renderer->clipRect(gfx::Rect{0, 0, _size.width, _size.height});

    gfx::Paint paint = gfx::Paint::create()
        .setStyle(gfx::Style::FILL_STYLE)
        .setColor(_color);

    renderer->drawRect(gfx::Rect{0, 0, _size.width, _size.height}, paint);

    renderer->restore();

    if (_child != nullptr) {
        _child->render(renderer, _childPosition.add(offset));
    }
}

} // ui