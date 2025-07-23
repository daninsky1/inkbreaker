#pragma once

#include <SDL3/SDL_events.h>

#include <include/core/SkSurface.h>
#include <include/core/SkData.h>
#include <include/core/SkImage.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkPath.h>

namespace ui
{

int runApplication(ui::Window& rootWindow)
{
    // SDL_InitFlags initFlags = SDL_INIT_VIDEO | SDL_INIT_EVENTS;
    // if (!SDL_Init(initFlags)) {
    //     SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
    //     return -1; // Initialization failed
    // }
    
    // SDL_WindowFlags windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    // SDL_Window* window = nullptr;
    // SDL_Renderer* renderer = nullptr;
    
    // if (!SDL_CreateWindowAndRenderer("Inkbreaker", 800, 600, windowFlags, &window, &renderer)) {
    //     SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
    //     return -1; // Window creation failed
    // }
    
    bool running = true;
    while (running) {
        ui::Event event;
        while (SDL_PollEvent(&event.sdlEvent)) {
            SDL_Log("Event type: %d", event.sdlEvent.type);
            
            bool isQuit = event.sdlEvent.type == SDL_EVENT_QUIT;
            std::cout << isQuit << std::endl;
            if (event.sdlEvent.type == SDL_EVENT_QUIT) {
                running = false; // Exit the loop on quit event
            }
        
            rootWindow.eventHandler(event); // Dispatch event to the widget
            rootWindow.render(); // Render the root window
        }
        
        
        // SDL_Renderer* renderer = rootWindow.getRenderer();
        
        // SDL_SetRenderTarget(renderer, nullptr);
        // SDL_RenderTexture(renderer, rootWindow._texture, nullptr, nullptr);
        // SDL_RenderPresent(renderer); // Present the rendered crendererontent
    }
    
    // Cleanup
    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0; // Success
}

} // namespace ui


void draw() {
    sk_sp<SkSurface> rasterSurface = SkSurface::MakeRaster(SkImageInfo::MakeN32Premul(512, 512));
    SkCanvas* canvas = rasterSurface->getCanvas();
    canvas->drawColor(SK_ColorWHITE);

    SkPaint paint;
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setStrokeWidth(8);
    paint.setColor(0xff4285F4);
    paint.setAntiAlias(true);
    paint.setStrokeCap(SkPaint::kRound_Cap);

    SkPath path;
    path.moveTo(10, 10);
    path.quadTo(256, 64, 128, 128);
    path.quadTo(10, 192, 250, 250);
    canvas->drawPath(path, paint);
}