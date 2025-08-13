//
// Created by daniel on 29/07/2025.
//

#pragma once
#include "widget.h"

namespace ui {

class ConstrainedBox : public SingleChildWidget {
public:
    ConstrainedBox(BoxConstraints boxConstraints, Widget* child) {
        _boxConstraints = boxConstraints;
        _child = child;
    }

    Size layout(const BoxConstraints& boxConstraints);
    void render(gfx::Renderer* renderer, Position offset);

protected:

};

} // ui
