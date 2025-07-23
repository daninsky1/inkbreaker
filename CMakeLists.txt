cmake_minimum_required(VERSION 3.26)

project(InkBreaker VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF)

add_executable(
        inkbreaker
        src/main.cpp
        src/ui/widget.cpp
        src/ui/window.cpp
        src/ui/container.cpp
)

# SDL 3
include(FetchContent)
FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.2.18
)
FetchContent_MakeAvailable(SDL3)
target_link_libraries(inkbreaker PRIVATE SDL3::SDL3-static)

# Skia
set(PKG_CONFIG_EXECUTABLE C:/msys64/clang64/bin/pkgconf.exe)
find_package(PkgConfig REQUIRED)
pkg_check_modules(Skia REQUIRED skia)

message(STATUS "Skia include dirs: ${Skia_INCLUDE_DIRS}")
message(STATUS "Skia lib dirs: ${Skia_LIBRARY_DIRS}")
message(STATUS "Skia libs: ${Skia_LIBRARIES}")

target_include_directories(inkbreaker SYSTEM PRIVATE ${Skia_INCLUDE_DIRS})
target_link_libraries(inkbreaker PRIVATE ${Skia_LIBRARIES})

# Executável de testes
# add_executable(
#         workbench
#         tests/workbench.cpp
# )
# target_link_libraries(workbench PRIVATE SDL3::SDL3)
# target_include_directories(workbench SYSTEM PRIVATE ${Skia_INCLUDE_DIRS})
# target_link_libraries(workbench PRIVATE ${Skia_LIBRARIES})

