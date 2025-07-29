// The main reason to use SDL callbacks is to avoid the annoying bug on some
// shitty platforms like Windows that blocks the main thread when dragging the
// window.
// issues:
// https://github.com/libsdl-org/SDL/issues/9102
// https://github.com/libsdl-org/SDL/issues/1059
// solution:
// https://wiki.libsdl.org/SDL3/README-main-functions
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "ui/framework.h"
#include "ui/examples.h"
#include "ui/window.h"
#include "ui/layout.h"

static ExempleApp* rootWindow = nullptr;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
    rootWindow = new ExempleApp();
    // rootWindow->setCurrentExemple(7);
    ui::BoxSpace boxSpace{.top = 0, .left = 0 , .bottom = 0, .right = 0};

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *sdlEvent)
{
    ui::Event event;
    event.sdlEvent = *sdlEvent; // Copy the SDL event to our Event structure

    if (event.sdlEvent.type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    rootWindow->eventHandler(event); // Dispatch event to the widget

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    uint64_t startTime = SDL_GetTicks(); // Get the current time in milliseconds
    rootWindow->update();
    rootWindow->render(nullptr, 0, 0); // Render the root window

    uint64_t elapsedTime = SDL_GetTicks() - startTime; // Calculate elapsed time

    // SDL_Log("Delta time: %ums", elapsedTime);
    if (elapsedTime < FRAME_DURATION_MS) {
        SDL_Delay(FRAME_DURATION_MS - elapsedTime); // Delay to maintain target FPS
    }
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{

}