# How to build
The project uses Cmake to build the project.

## Window
You need Msys2 [https://www.msys2.org/](https://www.msys2.org/) to build the project on Windows.
Recommended to use the Clang64 MinGW shell environment to build the project, because Skia has better support for Clang.

You need to install cmake, pkgconfig and Skia and Skia dependencies in the MinGW environment.
```shell
pacman -S mingw-w64-clang-x86_64-pkgconf \
    mingw-w64-clang-x86_64-cmake \
    mingw-w64-clang-x86_64-skia \
    mingw-w64-clang-x86_64-icu \
    mingw-w64-clang-x86_64-harfbuzz \
    mingw-w64-clang-x86_64-freetype \
    mingw-w64-clang-x86_64-libpng \
    mingw-w64-clang-x86_64-zlib
```

# Hellish Windows Build
https://groups.google.com/g/skia-discuss/c/IUo1AQowhhs?pli=1

bin\gn.exe gen out/Debug --args="is_debug=true extra_cflags=[\"/MDd\"]"
bin\gn.exe gen out/Release --args="is_official_build=true"

