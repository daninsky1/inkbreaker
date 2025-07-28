//
// Created by daniel on 27/07/2025.
//

#include "align.h"

namespace ui
{
Align::Align()
{

}


Size Align::layout(const BoxConstraint& constraint)
{
    BoxConstraint childConstraint = {
        .minWidth = 0,
        .minHeight = 0,
        .maxWidth = constraint.maxWidth,
        .maxHeight = constraint.maxHeight
    };
    if (_child != nullptr) {
        Size childSize = _child->layout(childConstraint);

        uint32_t x = 0;
        uint32_t y = 0;

        switch (_alignment) {
        case Alignment::TopLeft:
            x = 0;
            y = 0;
            break;
        case Alignment::TopCenter:
            x = (constraint.maxWidth - childSize.width) / 2;
            y = 0;
            break;
        case Alignment::TopRight:
            x = constraint.maxWidth - childSize.width;
            y = 0;
            break;
        case Alignment::CenterLeft:
            x = 0;
            y = (constraint.maxHeight - childSize.height) / 2;
            break;
        case Alignment::Center:
            x = (constraint.maxWidth - childSize.width) / 2;
            y = (constraint.maxHeight - childSize.height) / 2;
            break;
        case Alignment::CenterRight:
            x = constraint.maxWidth - childSize.width;
            y = (constraint.maxHeight - childSize.height) / 2;
            break;
        case Alignment::BottomLeft:
            x = 0;
            y = constraint.maxHeight - childSize.height;
            break;
        case Alignment::BottomCenter:
            x = (constraint.maxWidth - childSize.width) / 2;
            y = constraint.maxHeight - childSize.height;
            break;
        case Alignment::BottomRight:
            x = constraint.maxWidth - childSize.width;
            y = constraint.maxHeight - childSize.height;
            break;
        }

        _child->setPosition({ .x = x, .y = y });
    }
    // Normalize Align size
    return normalize(constraint);
}

void Align::render(SkCanvas* canvas, uint32_t offsetX, uint32_t offsetY)
{
    int absX = _position.x + offsetX;
    int absY = _position.y + offsetY;

    // Salva o estado atual do canvas
    canvas->save();

    // Aplica a translação para o container
    canvas->translate(_position.x, _position.y);
    //
    // // Define o retângulo de clipping do container
    canvas->clipRect(SkRect::MakeWH(_size.width, _size.height));

    // Cria a tinta (SkPaint) com estilo de preenchimento
    SkPaint paint;
    paint.setStyle(SkPaint::kFill_Style);
    paint.setColor(_color); // ou 0xFFFF0000 para vermelho opaco
    paint.setAntiAlias(true);

    // Desenha um retângulo preenchido do canto superior esquerdo até as dimensões do widget
    canvas->drawRect(SkRect::MakeXYWH(0, 0, _size.width, _size.height), paint);

    // Restaura o estado anterior do canvas
    canvas->restore();

    if (_child != nullptr) {
        _child->render(canvas, absX, absY);
    }
}


}   // namespace ui