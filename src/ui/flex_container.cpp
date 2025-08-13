#include "flex_container.h"

#include "SDL3/SDL_log.h"

namespace ui
{
void FlexContainer::render(gfx::Renderer* renderer, Position offset)
{
    switch (_flexDirection) {
    case css::FlexDirection::Row:
        SDL_Log("Rendering FlexContainer in Row direction");
        for (auto &child : _children) {        
            if (child) {
                child->render(renderer, {0, 0});
            }
            // absX += child->getWidth();
        }
        break;
    case css::FlexDirection::Column:
        SDL_Log("Rendering FlexContainer in Column direction");
        break;
    case css::FlexDirection::RowReverse:
        SDL_Log("Rendering FlexContainer in RowReverse direction");
        if (_children.empty()) return;
        
        for (size_t i = (_children.size() - 1); i > 0; i--) {        
            auto &child = _children[i];
            if (child) {
                child->render(renderer, {0, 0});
            }
            // absX += child->getWidth();
        }
        break;
    case css::FlexDirection::ColumnReverse:
        SDL_Log("Rendering FlexContainer in ColumnReverse direction");
        break;
    }
}

}