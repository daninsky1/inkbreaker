#include "widget.h"

namespace ui
{

Size Widget::normalize(const BoxConstraints& constraint)
{
    _size = constraint.clamp(_size);
    return _size;
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
