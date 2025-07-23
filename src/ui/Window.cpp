/*
* Created by Daniel S. S. on 19/07/2025.
*/

#include "window.h"

namespace ui {
Window::Window(std::string title, int w, int h, SDL_WindowFlags flags)
    : SingleChildWidget(0), _title(std::move(title)), _flags(flags)
{    
    _width = w;
    _height = h;
    if (!SDL_CreateWindowAndRenderer(_title.c_str(), _width, _height, _flags, &_window, &_renderer)) {
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

void Window::render()
{
    if (_child == nullptr) {
        SDL_Log("No child widget to render");
        return;
    }
    _child->render(_rasterSurface->getCanvas(), _x, _y);
    SDL_UpdateWindowSurface(_window);
}

Event& Window::eventHandler(Event& event)
{
    // Handle events specific to the window, e.g., resizing, closing, etc.
    switch (event.sdlEvent.type) {
    case SDL_EVENT_WINDOW_RESIZED:
        SDL_Log("Window resized to %d x %d", event.sdlEvent.window.data1, event.sdlEvent.window.data2);
        setRenderSurface();
        event.handled = true; // Mark the event as handled
        return event;
    case SDL_EVENT_QUIT:
        SDL_Log("Window close requested");
        event.handled = true; // Mark the event as handled
        return event;
    }
    
    // Call the parent class's event handler
    return SingleChildWidget::eventHandler(event);
}

} // ui