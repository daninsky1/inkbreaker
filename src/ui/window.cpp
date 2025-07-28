/*
* Created by Daniel S. S. on 19/07/2025.
*/

#include "window.h"

namespace ui {
Window::Window(std::string title, uint32_t w, uint32_t h, SDL_WindowFlags flags)
    : _title(std::move(title)), _flags(flags)
{
    _size = { w, h };
    _boxConstraint = BoxConstraint{w, h, w, h};
    if (!SDL_CreateWindowAndRenderer(_title.c_str(), w, h, _flags, &_window, &_renderer)) {
        SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
    }
    setRenderSurface();
}

void Window::setRenderSurface()
{
    _sdlSurface = SDL_GetWindowSurface(_window);
    SkImageInfo info = SkImageInfo::MakeN32Premul(_sdlSurface->w, _sdlSurface->h);
    size_t rowBytes = info.minRowBytes();
    _rasterSurface = SkSurface::MakeRasterDirect(info, _sdlSurface->pixels, rowBytes);
    SkCanvas* canvas = _rasterSurface->getCanvas();
    canvas->clear(_backgroundColor);
    // _texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _width, _height);
}

void Window::render(SkCanvas* canvas, uint32_t offsetX, uint32_t offsetY)
{
    if (_child == nullptr) {
        SDL_Log("No child widget to render.");
        return;
    }
    _child->render(_rasterSurface->getCanvas(), 0, 0);
    SDL_UpdateWindowSurface(_window);
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
        _boxConstraint = BoxConstraint(w, h, w, h);
        setRenderSurface();
        event.handled = true; // Mark the event as handled
        return event;
    }
    }
    
    // Call the parent class's event handler
    return SingleChildWidget::eventHandler(event);
}

} // ui 