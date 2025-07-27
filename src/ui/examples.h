//
// Created by daniel on 26/07/2025.
//

#pragma once

#include "center.h"
#include "widget.h"
#include "container.h"

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
