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

Size Container::layout(const BoxConstraint& constraint)
{
    if (_child != nullptr) {
        // Assumes the size of the child
        _size = _child->layout(constraint);
    } else if (!_size.hasSize()) {
        // Assumes the max size of the parent, this can lead to a bug, if later
        // be decided to implements the unrestricted widget, there will be no
        // reasable max size to get from constraint
        _size = {constraint.maxWidth, constraint.maxHeight};
    }
    // Implementação de padding dentro do container
    _size.width += (_padding.left + _padding.right);
    _size.height += (_padding.top + _padding.bottom);

    return normalize(constraint);
}

void Container::render(SkCanvas *canvas, uint32_t offsetX, uint32_t offsetY)
{
    // Salva o estado atual do canvas
    canvas->save();

    // Aplica a translação para o container
    canvas->translate(offsetX, offsetY);
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
        _child->render(canvas, offsetX + _padding.left, offsetY + _padding.top);
    }
}
} // namespace ui
