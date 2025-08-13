//
// Created by daniel on 29/07/2025.
//

#include "constrained_box.h"

namespace ui {


Size ConstrainedBox::layout(const BoxConstraints& boxConstraints)
{
    BoxConstraints tightened = _boxConstraints.tighten(boxConstraints);
    if (_child != nullptr) {
        _size = _child->layout(tightened);
    }
    return normalize(boxConstraints);
}

void ConstrainedBox::render(gfx::Renderer* renderer, Position offset)
{
    if (_child != nullptr) {
        _child->render(renderer, offset);
    }
}

} // ui