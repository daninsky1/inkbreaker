@echo off

cl src/main.cpp ^
	src/main_window.cpp ^
	src/view2d.cpp ^
    src/tree.cpp ^
	src/objects/shape.cpp ^
	src/objects/polygon.cpp ^
	src/objects/bezier.cpp ^
	src/edit_tools/edit_tool.cpp ^
	src/edit_tools/polygon_tool.cpp ^
	src/edit_tools/bezier_tool.cpp ^
	src/io/io.cpp ^
	src/objects/shapes.cpp ^
    /Feinkbreaker ^
    /Isrc ^
    /IC:vendor/fltk ^
    /Zi ^
    /MDd ^
    /std:c++14 ^
    /DWIN32 ^
    /D_WINDOWS ^
    /link ^
    /LIBPATH:vendor/fltk/build/lib/Debug ^
    fltkd.lib ^
    user32.lib ^
    comctl32.lib ^
    comdlg32.lib ^
    shell32.lib ^
    gdiplus.lib ^
    gdi32.lib ^
    ws2_32.lib ^
    ole32.lib ^
    winspool.lib ^
