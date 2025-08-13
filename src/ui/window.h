/*
 * Created by Daniel S. S. on 19/07/2025.
 */

#pragma once

#include "widget.h"

#include <SDL3/SDL.h>

#include "graphics/image_info.h"
#include "graphics/surface.h"
#include "graphics/color.h"

namespace ui {

class Window : public SingleChildWidget
{
public:
    Window(std::string title, int32_t w, int32_t h, SDL_WindowFlags flags);
    ~Window() override = default;

    void update() const;

    Event& eventHandler(Event& event) override;
    Size layout(const BoxConstraints& constraint) override { return _size; };
    // TODO(Daniel S): Analisar se é necessário manejar skia buffer fora desta
    //  classe
    void render(gfx::Renderer* renderer, Position offset) override;
protected:
    std::string _title;
    SDL_WindowFlags _flags;
    SDL_Window* _window = nullptr;
    SDL_Renderer* _sdlRenderer = nullptr;
    SDL_Surface* _sdlSurface = nullptr;
    gfx::Renderer* _renderer = nullptr;
    gfx::ImageInfo _imageInfo;
    std::shared_ptr<gfx::Surface> _surface;
    gfx::Color _color = gfx::Colors::GRAY;
    SDL_Texture* _texture{};
    void setRenderSurface();
};

} // ui