#pragma once

#define INKBREAKER_VERSION_MAJOR 0
#define INKBREAKER_VERSION_MINOR 0
#define INKBREAKER_VERSION_PATCH 1

#include "shapes.h"

//struct Space2D {
//    Vector2f offset;
//};

struct InkbreakerState {
    bool changed;
    Shape *active_selection;
    std::vector<Shape*> latest_selections;
};
