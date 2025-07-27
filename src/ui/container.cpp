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

Size Container::layout(const BoxConstraint& constraint)
{
    if (_child != nullptr) {
        // TODO(Daniel S.): Recalcular constraint margin e padding
        _size = _child->layout(constraint);
    }
    return normalize(constraint);
}

void Container::render(SkCanvas *canvas, uint32_t offsetX, uint32_t offsetY)
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
}
} // namespace ui