#include "widget.h"

namespace ui
{

void Widget::setPosition(const Position& position)
{
    _position = position;
}
void Widget::setSize(const Size& size)
{
    _size = size;
}

void Widget::setConstraint(uint32_t xConstraint, uint32_t yConstraint,
                           uint32_t widthConstraint,
                           uint32_t heightConstraint)
{

}

std::string Widget::toString() const { return _runtimeType; }

std::string Widget::getRuntimeType()
{
    if (_runtimeType.empty()) {
        // _runtimeType = std::format("{} Widget", static_cast<void*>(this));
        _runtimeType = typeid(*this).name();
    }
    return _runtimeType;
}

SingleChildWidget& SingleChildWidget::setChild(Widget& child)
{
    _needsUpdate = true;
    _child = &child;
    _child->setParent(this);
    return *this;
}

Widget* SingleChildWidget::getChild()
{
    return _child;
}
}   // ui
