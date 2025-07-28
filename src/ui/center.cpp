//
// Created by daniel on 26/07/2025.
//

#include "center.h"

namespace ui {

Center::Center()
{

}

Size Center::layout(const BoxConstraint& constraint)
{
    BoxConstraint centerConstraint = {
        .minWidth = 0,
        .minHeight = 0,
        .maxWidth = constraint.maxWidth,
        .maxHeight = constraint.maxHeight
    };
    if (_child != nullptr) {
        Size childSize = _child->layout(centerConstraint);
        _child->setPosition({
            .x = (constraint.maxWidth - childSize.width) / 2,
            .y = (constraint.maxHeight - childSize.height) / 2
        });
    }
    // Normalize Center size
    return normalize(constraint);
}

void Center::render(SkCanvas* canvas, uint32_t offsetX, uint32_t offsetY)
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
    paint.setColor(_backgroundColor); // ou 0xFFFF0000 para vermelho opaco
    paint.setAntiAlias(true);

    // Desenha um retângulo preenchido do canto superior esquerdo até as dimensões do widget
    canvas->drawRect(SkRect::MakeXYWH(0, 0, _size.width, _size.height), paint);

    // Restaura o estado anterior do canvas
    canvas->restore();

    if (_child != nullptr) {
        _child->render(canvas, absX, absY);
    }
}

} // ui