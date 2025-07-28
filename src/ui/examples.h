//
// Created by daniel on 26/07/2025.
//

#pragma once

#include <vector>

#include <SDL3/SDL_video.h>

#include "widget.h"
#include "container.h"
#include "center.h"
#include "align.h"

constexpr int W_WIDTH = 800;
constexpr int W_HEIGHT = 600;
constexpr int W_FLAGS = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

inline ui::Window* runExample(ui::Widget* widget)
{
    ui::Window* window = new ui::Window(
        "Inkbreaker",
        W_WIDTH, W_HEIGHT,
        W_FLAGS
    );
    window->setChild(*widget);
    return window;
}

/**
 * These examples mimics some Flutter behaviors and examples.
 */

/**
 * The window is the parent of the Container, and forces the Container to be
 * exactly the same size as the window.
 * So the Container fills the entire window and renders a red background color.
 * @retusrn
 */
inline ui::Widget* example1()
{
    ui::Container* container = new ui::Container();
    container->setColor(SkColors::kRed);
    return container;
}

/**
 * The red Container is set to a specific size of 200x200 pixels, but it cannnot
 * because the Window forces it to be exactly the same size as the screen.
 * So the Container fills the screen again.
 * @return
 */
inline ui::Widget* example2()
{
    ui::Container* container = new ui::Container();
    container->setColor(SkColors::kRed);
    container->setSize({200, 200});

    return container;
}

/**
 * The Window tells the Center widget to be exactly the same size as itself,
 * so the Center expands to fill the entire window.
 *
 * The Center then tells the Container that if can be any size if wants, as long
 * as it does not exceed the Center's size. So the Container can be, for
 * example 200x200
 * @return
 */
inline ui::Widget* example3()
{
    ui::Container* container = new ui::Container();
    container->setColor(SkColors::kRed);
    container->setSize({200, 200});

    ui::Center* center = new ui::Center();
    center->setColor(SkColors::kGray);
    center->setChild(*container);

    return center;
}

/**
 * The Align widget also expands to the size of the Window and tells its child
 * (the Container) that it can be any size he wants, as long as it doesn't
 * exceed the Align's size.
 *
 * Unlike Center, Align is responsible for positioning its child at fixed
 * alignment points such as bottom-right, top-left, center, etc.
 * @return
 */
inline ui::Widget* example4()
{
    ui::Container* container = new ui::Container();
    container->setColor(SkColors::kRed);
    container->setSize({200, 200});

    ui::Align* align = new ui::Align();
    align->setColor(SkColors::kGray);
    align->setAlignment(ui::Alignment::BottomRight);
    align->setChild(*container);

    return align;
}

/**
 * The Window forces the Center to be exactly the same size as the window, so
 * the Center fills the screen.
 *
 * The Center tells the Container that it can be any size it wants, but no
 * bigger than the screen. The Container wants to be of infinite size, but since
 * it can't be bigger than the window, it just fills the screen
 * @return
 */
inline ui::Widget* example5()
{
    ui::Container* container = new ui::Container();
    container->setColor(SkColors::kRed);
    container->setSize({std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::max()});

    ui::Center* center = new ui::Center();
    center->setColor(SkColors::kGray);
    center->setChild(*container);

    return center;
}

/**
 * The screen forces the Center to be exactly the same size as the window, so
 * the Center fills the window.
 *
 * The Center tells the Container that it can be any size it wants, but not
 * bigger than the screen.
 *
 * Since the Container has no child and no fixed size, it decides it wants to be
 * as big as possible, so it fills the whole screen
 *
 * But why does the Container decide that? Simply becausse a design decision to
 * behave like Flutter Container, that's it!
 * @return
 */
inline ui::Widget* example6()
{
    ui::Container* container = new ui::Container();
    container->setColor(SkColors::kRed);

    ui::Center* center = new ui::Center();
    center->setColor(SkColors::kGray);
    center->setChild(*container);

    return center;
}

/**
 *
 * @return
 */
inline ui::Widget* example7()
{
    ui::Container* greenContainer = new ui::Container();
    greenContainer->setSize({60, 60});
    greenContainer->setColor(SkColors::kGreen);

    ui::Container* container = new ui::Container();
    container->setColor(SkColors::kRed);
    container->setChild(*greenContainer);

    ui::Center* center = new ui::Center();
    center->setColor(SkColors::kGray);
    center->setChild(*container);

    return center;
}

class ExempleApp : public ui::Window
{
public:
    ExempleApp()
    :Window("", W_WIDTH , W_HEIGHT, W_FLAGS)
    {
        examplesDescription = {
            "Red Container constrained by Window",
            "Fixed-size Container constrained by the Window",
            "Centered fixed-size Container inside the Window",
            "Bottom-right aligned fixed-size Container",
            "Max-size Container constrained by the Window",
            "Unconstrained Container expansion",
            "Nested Container with wrapping behavior"

        };
        SDL_SetWindowTitle(_window, std::format("Examples. {}", examplesDescription[0]).c_str());
        examples = {
            example1(),
            example2(),
            example3(),
            example4(),
            example5(),
            example6(),
            example7()
        };
        _child = examples[currentExample];
    }

    ui::Event& eventHandler(ui::Event& event) override {
        event = Window::eventHandler(event);
        if (event.handled) return event;

        SDL_Event sdlEvent = event.sdlEvent;
        switch (sdlEvent.type) {
        case SDL_EVENT_KEY_DOWN:
            const SDL_KeyboardEvent& keyboardEvent = sdlEvent.key;
            if (keyboardEvent.repeat == 0) {
                switch (keyboardEvent.key) {
                case SDLK_RIGHT:
                    event.handled = true;
                    if (++currentExample > (examples.size() - 1)) currentExample = 0;
                    std::cout << currentExample << std::endl;
                    _child = examples[currentExample];
                    SDL_SetWindowTitle(_window, std::format("Examples. {}", examplesDescription[currentExample]).c_str());
                    break;
                case SDLK_LEFT:
                    event.handled = true;
                    if (--currentExample < 0) currentExample = examples.size() - 1;
                    std::cout << currentExample << std::endl;
                    _child = examples[currentExample];
                    SDL_SetWindowTitle(_window, std::format("Examples. {}", examplesDescription[currentExample]).c_str());
                    break;
                }
            }
            break;
        }
        return event;
    }

    std::vector<Widget*> examples;
    std::vector<std::string> examplesDescription;
    int currentExample = 0;
};
