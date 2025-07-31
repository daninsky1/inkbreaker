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
    Window(std::string title, int32_t w, int32_t h, SDL_WindowFlags flags);
    ~Window() override = default;
    
    SDL_Renderer* getRenderer() const {
        return _renderer;
    }

    void update() {
        if (_child != nullptr) {
            _child->layout(_boxConstraints);
        }
    }

    Event& eventHandler(Event& event) override;
    Size layout(const BoxConstraints& constraint) override { return _size; };
    // TODO(Daniel S): Analisar se é necessário manejar skia buffer fora desta
    //  classe
    void render(SkCanvas* canvas, Position offset) override;
protected:
    std::string _title;
    SDL_WindowFlags _flags;
    SDL_Window* _window = nullptr;
    SDL_Renderer* _renderer = nullptr;
    SDL_Surface* _sdlSurface = nullptr;
    SkImageInfo _skImageInfo;
    sk_sp<SkSurface> _rasterSurface;
    SkColor4f _color = SkColors::kGray;
    SDL_Texture* _texture;
    void setRenderSurface();
};

} // ui