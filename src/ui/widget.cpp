#include "widget.h"

namespace ui
{

std::string Widget::toString() const
{ 
    return _runtimeType;
}

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
    this->_child = &child;
    return *this;
}

Widget* SingleChildWidget::getChild()
{
    return _child;
}
}   // ui
