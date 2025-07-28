//
// Created by daniel on 26/07/2025.
//

#pragma once

#include "widget.h"
#include "container.h"
#include "center.h"
#include "align.h"

constexpr int width = 800;
constexpr int height = 600;

inline ui::Window* getMainWindow()
{
    return new ui::Window(
        "Inkbreaker",
        width, height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
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
inline ui::Window* example1()
{
    ui::Container* container = new ui::Container();
    container->setColor(SkColors::kRed);

    ui::Window* window = getMainWindow();
    window->setChild(*container);
    return window;
}

/**
 * The red Container is set to a specific size of 200x200 pixels, but it cannnot
 * because the Window forces it to be exactly the same size as the screen.
 * So the Container fills the screen again.
 * @return
 */
inline ui::Window* example2()
{
    ui::Container* container = new ui::Container();
    container->setColor(SkColors::kRed);
    container->setSize({200, 200});

    ui::Window* window = getMainWindow();
    window->setChild(*container);
    return window;
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
inline ui::Window* example3()
{
    ui::Container* container = new ui::Container();
    container->setColor(SkColors::kRed);
    container->setSize({200, 200});

    ui::Center* center = new ui::Center();
    center->setColor(SkColors::kGray);
    center->setChild(*container);

    ui::Window* window = getMainWindow();
    window->setChild(*center);
    return window;
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
inline ui::Window* example4()
{
    ui::Container* container = new ui::Container();
    container->setColor(SkColors::kRed);
    container->setSize({200, 200});

    ui::Align* align = new ui::Align();
    align->setColor(SkColors::kGray);
    align->setAlignment(ui::Alignment::BottomRight);
    align->setChild(*container);

    ui::Window* window = getMainWindow();
    window->setChild(*align);
    return window;
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
inline ui::Window* example5()
{
    ui::Container* container = new ui::Container();
    container->setColor(SkColors::kRed);
    container->setSize({std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::max()});

    ui::Center* center = new ui::Center();
    center->setColor(SkColors::kGray);
    center->setChild(*container);

    ui::Window* window = getMainWindow();
    window->setChild(*center);
    return window;
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
inline ui::Window* example6()
{
    ui::Container* container = new ui::Container();
    container->setColor(SkColors::kRed);

    ui::Center* center = new ui::Center();
    center->setColor(SkColors::kGray);
    center->setChild(*container);

    ui::Window* window = getMainWindow();
    window->setChild(*center);
    return window;
}

/**
 *
 * @return
 */
inline ui::Window* example7()
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

    ui::Window* window = getMainWindow();
    window->setChild(*center);
    return window;
}