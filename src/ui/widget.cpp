#include "widget.h"

namespace ui
{

Size Widget::normalize(const BoxConstraint& constraint)
{
    _size.width = std::clamp(_size.width, constraint.minWidth, constraint.maxWidth);
    _size.height = std::clamp(_size.height, constraint.minHeight, constraint.maxHeight);
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
