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
#include "constrained_box.h"
#include "text.h"
#include "unconstrained_box.h"

constexpr int W_WIDTH = 800;
constexpr int W_HEIGHT = 600;
constexpr int W_FLAGS = SDL_WINDOW_RESIZABLE;

/**
 * These examples mimics some Flutter behaviors and examples.
 * https://docs.flutter.dev/ui/layout/constraints
 *
 * @param widget
 * @return
 */
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
 * The window is the parent of the Container, and forces the Container to be
 * exactly the same size as the window.
 * So the Container fills the entire window and renders a red background color.
 * @retusrn
 */
inline ui::Widget* example1()
{
    ui::Container* container = new ui::Container();
    container->setColor(gfx::Colors::RED);
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
    container->setColor(gfx::Colors::RED);
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
    container->setColor(gfx::Colors::RED);
    container->setSize({200, 200});

    ui::Center* center = new ui::Center();
    center->setColor(gfx::Colors::GRAY);
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
    container->setColor(gfx::Colors::RED);
    container->setSize({200, 200});

    ui::Align* align = new ui::Align();
    align->setColor(gfx::Colors::GRAY);
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
    container->setColor(gfx::Colors::RED);
    container->setSize({std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::max()});

    ui::Center* center = new ui::Center();
    center->setColor(gfx::Colors::GRAY);
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
    container->setColor(gfx::Colors::RED);

    ui::Center* center = new ui::Center();
    center->setColor(gfx::Colors::GRAY);
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
    greenContainer->setColor(gfx::Colors::GREEN);

    ui::Container* container = new ui::Container();
    container->setColor(gfx::Colors::RED);
    container->setChild(*greenContainer);

    ui::Center* center = new ui::Center();
    center->setColor(gfx::Colors::GRAY);
    center->setChild(*container);

    return center;
}

inline ui::Widget* example8()
{
    ui::Container* greenContainer = new ui::Container();
    greenContainer->setSize({60, 60});
    greenContainer->setColor(gfx::Colors::GREEN);

    ui::Container* redContainer = new ui::Container();
    redContainer->setColor(gfx::Colors::RED);
    redContainer->setPadding({30, 30, 30, 30});
    redContainer->setChild(*greenContainer);

    ui::Center* center = new ui::Center();
    center->setColor(gfx::Colors::GRAY);
    center->setChild(*redContainer);

    return center;
}

inline ui::Widget* example9()
{
    ui::Container* redContainer = new ui::Container();
    redContainer->setColor(gfx::Colors::RED);

    ui::ConstrainedBox* constrainedBox = new ui::ConstrainedBox{
        {.minWidth = 70, .minHeight = 70, .maxWidth = 150, .maxHeight = 150},
        redContainer
    };
    return constrainedBox;
}

inline ui::Widget* example10()
{
    ui::Container* redContainer = new ui::Container();
    redContainer->setColor(gfx::Colors::RED);
    redContainer->setSize({.width = 10, .height = 10});

    ui::ConstrainedBox* constrainedBox = new ui::ConstrainedBox{
        {.minWidth = 70, .minHeight = 70, .maxWidth = 150, .maxHeight = 150},
        redContainer
    };

    ui::Center* center = new ui::Center();
    center->setColor(gfx::Colors::GRAY);
    center->setChild(*constrainedBox);

    return center;
}

inline ui::Widget* example11()
{
    ui::Container* redContainer = new ui::Container();
    redContainer->setColor(gfx::Colors::RED);
    redContainer->setSize({.width = 1000, .height = 1000});

    ui::ConstrainedBox* constrainedBox = new ui::ConstrainedBox{
            {.minWidth = 70, .minHeight = 70, .maxWidth = 150, .maxHeight = 150},
            redContainer
        };

    ui::Center* center = new ui::Center();
    center->setColor(gfx::Colors::GRAY);
    center->setChild(*constrainedBox);

    return center;
}

inline ui::Widget* example12()
{
    ui::Container* redContainer = new ui::Container();
    redContainer->setColor(gfx::Colors::RED);
    redContainer->setSize({.width = 100, .height = 100});

    ui::ConstrainedBox* constrainedBox = new ui::ConstrainedBox{
                {.minWidth = 70, .minHeight = 70, .maxWidth = 150, .maxHeight = 150},
                redContainer
            };

    ui::Center* center = new ui::Center();
    center->setColor(gfx::Colors::GRAY);
    center->setChild(*constrainedBox);

    return center;
}

/**
 * The window forces the UnconstrainedBox to be exactly the same size as the
 * screen. However, the UnconstrainedBox lets its child Container be any size
 * it wants
 * @return
 */
inline ui::Widget* example13()
{
    ui::Container* redContainer = new ui::Container();
    redContainer->setColor(gfx::Colors::RED);
    redContainer->setSize({.width = 40, .height = 100});

    ui::UnconstrainedBox* unconstrainedBox = new ui::UnconstrainedBox{
        redContainer
    };

    return unconstrainedBox;
}

/**
 * This example demonstrates the behavior of the UnconstrainedBox when its child exceeds
 * the available space.
 *
 * The UnconstrainedBox itself is constrained by the Window widget,
 * but it removes those constraints when passing them down to its child.
 * This means the child Container is allowed to have any size it wants.
 *
 * However, in this case, the child's width (4000) is much larger than the available width
 * provided by the screen or layout parent.
 * In Flutter, this would trigger an "overflow warning" in debug mode.
 * In this framework, such debug warnings are not yet implemented.
 *
 * This is useful for testing layout behavior in extreme sizing conditions and is
 * particularly relevant when planning debug tooling in the future.
 *
 * Note: The visual overflow indicator is not currently implemented.
 *
 * @return A widget tree with an UnconstrainedBox containing an oversized red Container.
 */
inline ui::Widget* example14()
{
    ui::Container* redContainer = new ui::Container();
    redContainer->setColor(gfx::Colors::RED);
    redContainer->setSize({.width = 4000, .height = 100});

    ui::UnconstrainedBox* unconstrainedBox = new ui::UnconstrainedBox{
        redContainer
    };

    return unconstrainedBox;
}

/**
 * This example demonstrates the behavior of the OverflowBox when its child exceeds
 * the available space. It creates a red Container with a width of 4000 pixels and a height of 100.
 *
 * The OverflowBox is constrained by its parent (e.g., the screen), but passes relaxed constraints
 * to its child. In this case, the child is allowed to have an infinite maximum width and height,
 * so it can render far beyond the visible area.
 *
 * Unlike UnconstrainedBox, OverflowBox does not produce any warnings when the child overflows.
 * It simply renders what it can within the clipping and visibility rules of the parent widget or surface.
 *
 * This behavior is useful when you want to allow a child to render beyond the bounds of its container
 * without being limited or triggering debug warnings.
 *
 * Note: As with example14, debug overflow indicators are not currently implemented in this framework.
 *
 * @return A widget tree with an OverflowBox containing an oversized red Container.
 */
inline ui::Widget* example15()
{
    ui::Container* redContainer = new ui::Container();
    redContainer->setColor(gfx::Colors::RED);
    redContainer->setSize({.width = 4000, .height = 100});

    ui::OVerflowBox* unconstrainedBox = new ui::OVerflowBox{
        redContainer
    };

    return unconstrainedBox;
}

/**
 * This example is intentionally left non-functional in this framework because it demonstrates
 * a limitation that does not apply here.
 *
 * In Flutter, this example fails to render because the UnconstrainedBox allows its child to
 * have any size, and the child Container tries to have an infinite width (`double.infinity`),
 * which leads to an error: "BoxConstraints forces an infinite width."
 *
 * However, in this framework, layout units are currently represented as integers, and infinite
 * values are not supported. Therefore, it is not possible to create a container with an infinite
 * width. This makes the issue shown in the Flutter example irrelevant here.
 *
 * The concept of "infinite size" is undefined in this context, as the rendering engine assumes
 * all dimensions are concrete, finite integers.
 *
 * In the future, support for floating-point dimensions (e.g., using `double`) might be considered,
 * but it is currently not planned or confirmed.
 *
 * @return nullptr — this example is not implemented due to framework design constraints.
 */
inline ui::Widget* example16()
{
    ui::Container* blackContainer = new ui::Container();
    blackContainer->setColor(gfx::Colors::BLACK);
    return blackContainer;
}

inline ui::Widget* example17()
{
    ui::Container* redContainer = new ui::Container();
    redContainer->setColor(gfx::Colors::RED);
    redContainer->setSize({.width = 4000, .height = 100});

    

    ui::UnconstrainedBox* unconstrainedBox = new ui::UnconstrainedBox{
        redContainer
    };

    return unconstrainedBox;
}

inline ui::Widget* example18()
{
    ui::Container* redContainer = new ui::Container();
    redContainer->setColor(gfx::Colors::BLACK);
    redContainer->setSize({.width = 40, .height = 50});
    return redContainer;
}

inline ui::Widget* example19()
{
    ui::Container* redContainer = new ui::Container();
    redContainer->setColor(gfx::Colors::BLACK);
    redContainer->setSize({.width = 40, .height = 50});
    return redContainer;
}

inline ui::Widget* example20()
{
    ui::Container* redContainer = new ui::Container();
    redContainer->setColor(gfx::Colors::BLACK);
    redContainer->setSize({.width = 40, .height = 50});
    return redContainer;
}

inline ui::Widget* example21()
{
    ui::Container* redContainer = new ui::Container();
    redContainer->setColor(gfx::Colors::BLACK);
    redContainer->setSize({.width = 40, .height = 50});
    return redContainer;
}

inline ui::Widget* example22()
{
    ui::Container* redContainer = new ui::Container();
    redContainer->setColor(gfx::Colors::BLACK);
    redContainer->setSize({.width = 40, .height = 50});
    return redContainer;
}

inline ui::Widget* example23()
{
    ui::Container* redContainer = new ui::Container();
    redContainer->setColor(gfx::Colors::BLACK);
    redContainer->setSize({.width = 40, .height = 50});
    return redContainer;
}

inline ui::Widget* example24()
{
    ui::Container* redContainer = new ui::Container();
    redContainer->setColor(gfx::Colors::BLACK);
    redContainer->setSize({.width = 40, .height = 50});
    return redContainer;
}

inline ui::Widget* example25()
{
    ui::Container* redContainer = new ui::Container();
    redContainer->setColor(gfx::Colors::BLACK);
    redContainer->setSize({.width = 40, .height = 50});
    return redContainer;
}

inline ui::Widget* example26()
{
    ui::Container* redContainer = new ui::Container();
    redContainer->setColor(gfx::Colors::BLACK);
    redContainer->setSize({.width = 40, .height = 50});
    return redContainer;
}

inline ui::Widget* example27()
{
    ui::Container* redContainer = new ui::Container();
    redContainer->setColor(gfx::Colors::BLACK);
    redContainer->setSize({.width = 40, .height = 50});
    return redContainer;
}

inline ui::Widget* example28()
{
    ui::Container* redContainer = new ui::Container();
    redContainer->setColor(gfx::Colors::BLACK);
    redContainer->setSize({.width = 40, .height = 50});
    return redContainer;
}

inline ui::Widget* example29()
{
    ui::Container* redContainer = new ui::Container();
    redContainer->setColor(gfx::Colors::BLACK);
    redContainer->setSize({.width = 40, .height = 50});
    return redContainer;
}

inline ui::Widget* textExample1()
{
    auto* text = new ui::Text("Hello, World!");
    auto* center = new ui::Center();
    center->setColor(gfx::Colors::BLACK);
    center->setChild(*text);
    return center;
}

class ExampleApp : public ui::Window
{
public:
    ExampleApp()
        :Window("", W_WIDTH , W_HEIGHT, W_FLAGS)
    {
        examplesDescriptions = {
            "1-Red Container constrained by Window",
            "2-Fixed-size Container constrained by the Window",
            "3-Centered fixed-size Container inside the Window",
            "4-Bottom-right aligned fixed-size Container",
            "5-Max-size Container constrained by the Window",
            "6-Unconstrained Container expansion",
            "7-Nested Container with wrapping behavior",
            "8",
            "9",
            "10",
            "11",
            "12",
            "13",
            "14",
            "15",
            "16",
            "17",
            "18",
            "19",
            "20",
            "21",
            "22",
            "23",
            "24",
            "25",
            "26",
            "27",
            "28",
            "29",
            "Text 01"
        };
        SDL_SetWindowTitle(_window, std::format("Examples. {}", examplesDescriptions[0]).c_str());
        examples = {
            example1(),
            example2(),
            example3(),
            example4(),
            example5(),
            example6(),
            example7(),
            example8(),
            example9(),
            example10(),
            example11(),
            example12(),
            example13(),
            example14(),
            example15(),
            example16(),
            example17(),
            example18(),
            example19(),
            example20(),
            example21(),
            example22(),
            example23(),
            example24(),
            example25(),
            example26(),
            example27(),
            example28(),
            example29(),
            textExample1()
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
                    setCurrentExemple(++currentExample);
                    break;
                case SDLK_LEFT:
                    event.handled = true;
                    setCurrentExemple(--currentExample);
                    break;
                }
            }
            break;
        }
        return event;
    }

    void setCurrentExemple(int index) {
        int last = (examples.size() - 1);
        std::cout << index << "   " << last << std::endl;
        if (index > last) currentExample = 0;
        else if (index < 0) currentExample = last;
        else currentExample = index;

        std::cout << currentExample << std::endl;

        _child = examples[currentExample];
        SDL_SetWindowTitle(_window, std::format("Examples. {}", examplesDescriptions[currentExample]).c_str());
    }

    std::vector<Widget*> examples;
    std::vector<std::string> examplesDescriptions;
    int currentExample = 0;
};
