#pragma once


enum class Mode {
    select,
    pan,
    zoom,
    draw
};

enum class Select {
    move,
    scale,
    rotate
};

enum class Draw {
    line,
    rect,
    circle,
    poly,
    polygon,
    bezier
};