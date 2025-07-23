#include <iostream>

#include <cstdint>

#include <SDL3/SDL_main.h>

#include "ui/widget.h"
#include "ui/window.h"
#include "ui/framework.h"
#include "ui/container.h"

constexpr int width = 800;
constexpr int height = 600;

int SDL_main(int argc, char* argv[])
{
    SDL_WindowFlags windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

    ui::Container container = ui::Container(0);
    
    ui::Window window("Inkbreaker", width, height, windowFlags);
    window.setChild(container);
    
    return ui::runApplication(window);
}
