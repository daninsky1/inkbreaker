#pragma once

#define INKBREAKER_VERSION_MAJOR 0
#define INKBREAKER_VERSION_MINOR 0
#define INKBREAKER_VERSION_PATCH 1

#include "objects/shapes.h"

//struct Space2D {
//    Vector2f offset;
//};

struct InkbreakerState {
    bool changed;
    old::Shape *active_selection;
    std::vector<old::Shape*> latest_selections;
};
