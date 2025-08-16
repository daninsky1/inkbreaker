/*
* Created by Daniel S. S. on 19/07/2025.
*/

#include "window.h"

namespace ui {
Window::Window(std::string title, int32_t w, int32_t h, SDL_WindowFlags flags)
    : _title(std::move(title)), _flags(flags)
{
    _size = { w, h };
    _boxConstraints = BoxConstraints{w, h, w, h};
    if (!SDL_CreateWindowAndRenderer(_title.c_str(), w, h, _flags, &_window, &_sdlRenderer)) {
        SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
    }
    setRenderSurface();
}

void Window::setRenderSurface()
{
    _sdlSurface = SDL_GetWindowSurface(_window);
    gfx::ImageInfo info{.dimension = {_sdlSurface->w, _sdlSurface->h}};
    _surface = gfx::Surface::create(info);
    if (_renderer == nullptr) {
        _renderer = gfx::Renderer::create();
    } else {
        delete _renderer;
    }
    _renderer->releaseRenderTarget();
    _renderer->bindRenderTarget(_surface);
    _renderer->clear(_color);
}

void Window::render(gfx::Renderer* renderer, Position offset)
{
    if (_child == nullptr) {
        SDL_Log("No child widget to render.");
        return;
    }
    _renderer->clear(_color);
    _child->render(_renderer, {0, 0});

    // NOTA(Daniel S.): Hack, muita cópia acontecendo
    _sdlSurface = SDL_GetWindowSurface(_window);
    SDL_Surface* surface = SDL_CreateSurface(
        _surface->getWidth(), _surface->getHeight(),
        SDL_PIXELFORMAT_ARGB8888
    );
    SDL_memcpy(
        surface->pixels,
        _surface->getData(),
        surface->h * surface->pitch
    );

    SDL_BlitSurface(surface, NULL, _sdlSurface, NULL);
    SDL_UpdateWindowSurface(_window);
    SDL_DestroySurface(surface);
}

void Window::update() const {
    if (_child != nullptr) {
        _child->layout(_boxConstraints);
    }
}

Event& Window::eventHandler(Event& event)
{
    // Handle events specific to the window, e.g., resizing, closing, etc.
    SDL_Event sdlEvent = event.sdlEvent;
    switch (sdlEvent.type) {
    case SDL_EVENT_WINDOW_RESIZED: {
        int w = sdlEvent.window.data1;
        int h = sdlEvent.window.data2;
        SDL_Log("Window resized to %d x %d", w, h);
        _size = Size(w, h);
        _boxConstraints = BoxConstraints(w, h, w, h);
        setRenderSurface();
        event.handled = true; // Mark the event as handled
        return event;
    }
    }
    
    // Call the parent class's event handler
    return SingleChildWidget::eventHandler(event);
}

} // ui 