#!/bin/sh

CC='zig c++ -lgdi32 -lcomdlg32'
# CC='clang'
# CC='gcc'

${CC} src/main.cpp \
	src/main_window.cpp \
	src/view2d.cpp \
    src/tree.cpp \
	src/objects/shape.cpp \
	src/objects/polygon.cpp \
	src/objects/bezier.cpp \
	src/edit_tools/edit_tool.cpp \
	src/edit_tools/polygon_tool.cpp \
	src/edit_tools/bezier_tool.cpp \
	src/io/io.cpp \
	src/objects/shapes.cpp \
    -o inkbreaker \
    -std=c++14 \
    -Isrc/ \
    -Ivendor/fltk/ \
    $(sh vendor/fltk/fltk-config --cflags) \
    -Lvendor/fltk/lib/ \
    $(sh vendor/fltk/fltk-config --ldstaticflags) \
    -lstdc++ \
    -lfltk \
    -lfltk_forms \
    -lfltk_images \
    -lfltk_png \
    -lfltk_gl \
    -lfltk_jpeg \
    -lfltk_z
