/*
 * Created by Daniel S. S. on 19/07/2025.
 */

#pragma once

#include <string>
#include <format>

#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_log.h>

#include <include/core/SkImageInfo.h>
#include <include/core/SkSurface.h>
#include <include/core/SkColor.h>

#include "widget.h"

namespace ui {

class Window : public SingleChildWidget
{
public:
    Window(std::string title, uint32_t w, uint32_t h, SDL_WindowFlags flags);
    ~Window() = default;
    
    SDL_Renderer* getRenderer() const {
        return _renderer;
    }

    void update() {
        if (_child != nullptr) {
            _child->layout(_boxConstraint);
        }
    }
    
    void render() override;

    Event& eventHandler(Event& event) override;
    
    SDL_Texture* _texture;
private:
    std::string _title;
    SDL_WindowFlags _flags;
    SDL_Window* _window = nullptr;
    SDL_Renderer* _renderer = nullptr;
    SDL_Surface* _sdlSurface = nullptr;
    SkImageInfo _skImageInfo;
    sk_sp<SkSurface> _rasterSurface;
    SkColor4f _backgroundColor = SkColors::kWhite;
    
    void setRenderSurface();
};

} // ui