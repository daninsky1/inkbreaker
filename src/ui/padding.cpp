//
// Created by daniel on 27/07/2025.
//

#include "padding.h"

namespace ui {

Padding::Padding()
{

}

Padding::Padding(uint32_t top, uint32_t left, uint32_t bottom, uint32_t right)
    :_padding(top, left, bottom, right)
{
}

Size Padding::layout(const BoxConstraints& constraint)
{
    // Reduz as constraints disponíveis para o filho
    BoxConstraints childConstraint = {
        .minWidth = std::max(0, constraint.minWidth > (_padding.left + _padding.right)
                             ? constraint.minWidth - (_padding.left + _padding.right)
                             : 0),
        .minHeight = std::max(0, constraint.minHeight > (_padding.top + _padding.bottom)
                              ? constraint.minHeight - (_padding.top + _padding.bottom)
                              : 0),
        .maxWidth = constraint.maxWidth > (_padding.left + _padding.right)
                    ? constraint.maxWidth - (_padding.left + _padding.right)
                    : 0,
        .maxHeight = constraint.maxHeight > (_padding.top + _padding.bottom)
                     ? constraint.maxHeight - (_padding.top + _padding.bottom)
                     : 0
    };

    Size childSize = { 0, 0 };
    if (_child != nullptr) {
        childSize = _child->layout(childConstraint);

        // Posiciona o filho com offset do padding
        _childPosition = { .x = _padding.left, .y = _padding.top };
    }

    // O próprio tamanho do Padding inclui o filho + padding
    return {
        .width = childSize.width + _padding.left + _padding.right,
        .height = childSize.height + _padding.top + _padding.bottom
    };
}

void Padding::render(gfx::Renderer* renderer, Position offset)
{

}
} // ui
