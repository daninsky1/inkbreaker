//
// Created by daniel on 29/07/2025.
//

#include "unconstrained_box.h"

namespace ui {

Size LimitedBox::layout(const BoxConstraints& boxConstraints)
{
    if (_child != nullptr) {
        Size childSize = _child->layout(_boxConstraints);

        int32_t x = 0;
        int32_t y = 0;

        switch (_alignment) {
        case Alignment::TopLeft:
            x = 0;
            y = 0;
            break;
        case Alignment::TopCenter:
            x = (boxConstraints.maxWidth - childSize.width) / 2;
            y = 0;
            break;
        case Alignment::TopRight:
            x = boxConstraints.maxWidth - childSize.width;
            y = 0;
            break;
        case Alignment::CenterLeft:
            x = 0;
            y = (boxConstraints.maxHeight - childSize.height) / 2;
            break;
        case Alignment::Center:
            x = (boxConstraints.maxWidth - childSize.width) / 2;
            y = (boxConstraints.maxHeight - childSize.height) / 2;
            break;
        case Alignment::CenterRight:
            x = boxConstraints.maxWidth - childSize.width;
            y = (boxConstraints.maxHeight - childSize.height) / 2;
            break;
        case Alignment::BottomLeft:
            x = 0;
            y = boxConstraints.maxHeight - childSize.height;
            break;
        case Alignment::BottomCenter:
            x = (boxConstraints.maxWidth - childSize.width) / 2;
            y = boxConstraints.maxHeight - childSize.height;
            break;
        case Alignment::BottomRight:
            x = boxConstraints.maxWidth - childSize.width;
            y = boxConstraints.maxHeight - childSize.height;
            break;
        }

        _childPosition = { .x = x, .y = y };
    }
    return normalize(boxConstraints);
}

void LimitedBox::render(gfx::Renderer* renderer, Position offset)
{
    if (_child != nullptr) {
        _child->render(renderer, _childPosition.add(offset));
    }
}

} // ui