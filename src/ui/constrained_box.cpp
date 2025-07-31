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

void ConstrainedBox::render(SkCanvas* canvas, Position offset)
{
    if (_child != nullptr) {
        _child->render(canvas, offset);
    }
}

} // ui