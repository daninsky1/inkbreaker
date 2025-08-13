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
#include "ui/window.h"
#include "ui/layout.h"
#include "graphics/geometry.h"
#include "graphics/render.h"
#include "graphics/renderer.h"
#include "graphics/paint.h"

using Shapes = std::vector<gfx::Shape>;

constexpr int W_WIDTH = 800, W_HEIGHT = 600;
constexpr SDL_WindowFlags W_FLAGS = SDL_WINDOW_OPENGL;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
std::shared_ptr<gfx::Surface> surface = nullptr;
SDL_Surface* sdlSurface = nullptr;
SDL_Surface* sdlWindowSurface = nullptr;
auto skImageInfo = SkImageInfo::MakeN32Premul(W_WIDTH, W_HEIGHT);
static Shapes shapes;

void sdlCheckOK(bool ok)
{
    if (!ok) {
        SDL_Log("SDL Error: %s", SDL_GetError());
    }
}

void sdlCheckPtr(void* ptr)
{
    if (ptr == nullptr) {
        SDL_Log("SDL Error: %s", SDL_GetError());
    }
}

gfx::Renderer* render = nullptr;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
    sdlCheckOK(
        SDL_CreateWindowAndRenderer("Teste",W_WIDTH, W_HEIGHT,
                                       W_FLAGS, &window, &renderer)
    );

    gfx::ImageInfo textureInfo{.dimension = {W_WIDTH, W_HEIGHT}};
    surface = gfx::Surface::create(textureInfo);

    render = gfx::Renderer::create();

    sdlSurface = SDL_CreateSurface(W_WIDTH, W_HEIGHT,SDL_PIXELFORMAT_ARGB8888);
    sdlCheckPtr(sdlSurface);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *sdlEvent)
{

    switch (sdlEvent->type) {
    case SDL_EVENT_QUIT: {
        return SDL_APP_SUCCESS;
        break;
    }
    case SDL_EVENT_WINDOW_RESIZED: {
        if (sdlSurface != nullptr) {
            sdlSurface = SDL_CreateSurface(W_WIDTH, W_HEIGHT,SDL_PIXELFORMAT_ARGB8888);
            sdlCheckPtr(sdlSurface);
        }
        gfx::ImageInfo textureInfo{.dimension = {W_WIDTH, W_HEIGHT}};
        surface = gfx::Surface::create(textureInfo);
        render->bindRenderTarget(surface);
        sdlWindowSurface = SDL_GetWindowSurface(window);
        break;
    }
    }

    sdlCheckOK(
        SDL_BlitSurface(sdlSurface, NULL, sdlSurface, NULL)
    );

    return SDL_APP_CONTINUE;
}

void BL_TEST();
SDL_AppResult SDL_AppIterate(void* appstate)
{
    uint64_t startTime = SDL_GetTicks(); // Get the current time in milliseconds
    render->bindRenderTarget(surface);

    gfx::Rect rect{0, 0, W_WIDTH, W_HEIGHT};
    gfx::Paint paint{};
    gfx::Color color{0, 255, 0, 255};
    paint.setColor(color);

    render->drawRect(rect, paint);

    render->releaseRenderTarget();

    // surface->writeToFile("teste.png");

    // BL_TEST();

    // return SDL_APP_SUCCESS;

    sdlSurface->pixels = surface->getData();

    SDL_Surface* testSurface = SDL_CreateSurface(W_WIDTH, W_HEIGHT,SDL_PIXELFORMAT_RGBA8888);
    if (!testSurface) {
        std::cout << "SDL_CreateRGBSurface Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return SDL_APP_FAILURE;
    }

    uint32_t packed =
        (static_cast<uint32_t>(255) << 24) |
        (static_cast<uint32_t>(0) << 16) |
        (static_cast<uint32_t>(255) <<  8) |
        (static_cast<uint32_t>(255) <<  0);

    SDL_FillSurfaceRect(testSurface, NULL, packed);

    sdlWindowSurface = SDL_GetWindowSurface(window);

    // Blit testSurface para windowSurface
    // SDL_BlitSurface(testSurface, NULL, sdlWindowSurface, NULL);

    SDL_BlitSurface(sdlSurface, NULL, sdlWindowSurface, NULL);
    SDL_UpdateWindowSurface(window);

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

void BL_TEST()
{
    BLImage img(W_WIDTH, W_HEIGHT, BL_FORMAT_PRGB32);

    // Attach a rendering context into `img`.
    BLContext ctx;

    ctx.begin(img);

    // Fill a path with opaque white - 0xAARRGGBB.
    ctx.fillRect(
        BLRectI(0, 0, W_WIDTH, W_HEIGHT),
        BLRgba32(0xFFFFFF00)
    );

    // Detach the rendering context from `img`.
    ctx.end();

    // Let's use some built-in codecs provided by Blend2D.
    img.writeToFile("bl_sample_1.png");
}