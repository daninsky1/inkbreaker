//
// Created by daniel on 29/07/2025.
//

#pragma once
#include "align.h"
#include "widget.h"

namespace ui {

class UnconstrainedBox : public SingleChildWidget {
public:
    UnconstrainedBox(Widget* child) {
        _boxConstraints = {
            .minWidth = 0,
            .minHeight = 0,
            .maxWidth =  std::numeric_limits<int32_t>::max(),
            .maxHeight = std::numeric_limits<int32_t>::max()
        };
        _child = child;
    }

    Size layout(const BoxConstraints& boxConstraints) override;
    void render(gfx::Renderer* renderer, Position offset) override;

protected:
    Alignment _alignment = Alignment::Center;
};
/**
 * UnconstrainedBox OVerflowBox is basically the same type here, unless
 */
typedef UnconstrainedBox OVerflowBox;

} // ui
