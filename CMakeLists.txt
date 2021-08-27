cmake_minimum_required(VERSION 3.16)

project(inkbreaker
        VERSION 0.1.0
        LANGUAGES C CXX)
configure_file(InkBreakerConfig.h.in InkBreakerConfig.h)

# toolchain var: -DCMAKE_TOOLCHAIN_FILE=[toolchain.cmake path]
# TODO: add MSVC fltk dir resolution
# please use the following names to the build dir on fltk
# MINGW: mingw/ for crosscompile or compile with mingw under windows
# MSVC: msvc/ for visual studio solution
# UNIX: build under unix using make
if(MSVC)
    add_compile_options(-W4)
    set(FLTK_DIR "vendor/fltk/msvc/")
elseif(MINGW)
    add_compile_options(-mwindows -lgdiplus)
    set(FLTK_DIR "vendor/fltk/mingw/")
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
    set(FLTK_DIR "vendor/fltk/build/")
endif()

# Find fltk package
find_package(FLTK REQUIRED NO_MODULE)

# stb header libraries
set(STB_PERLIN_LIB stb_perlin)
add_library(${STB_PERLIN_LIB} INTERFACE)
target_include_directories(${STB_PERLIN_LIB}
    INTERFACE vendor/)

# Main app
set(MAIN_APP_NAME InkBreaker)
set(MAIN_APP_SOURCES src/main.cpp
    src/canvas.cpp src/canvas.h
    screen_space.cpp screen_space.h
    world_space.cpp world_space.h)
add_executable(${MAIN_APP_NAME} WIN32 MACOSX_BUNDLE ${MAIN_APP_SOURCES}
set_property(TARGET ${MAIN_APP_NAME} PROPERTY CXX_STANDARD 17)
target_include_directories(${MAIN_APP_NAME}
                           PRIVATE ${PROJECT_BINARY_DIR}
                           PRIVATE ${FLTK_INCLUDE_DIRS}
)

target_link_libraries(${MAIN_APP_NAME}
    PRIVATE fltk
    PRIVATE fltk_forms
    PRIVATE fltk_gl
    PRIVATE fltk_images
    PRIVATE fltk_jpeg
    PRIVATE ${STB_PERLIN_LIB}
)
if(APPLE)
    target_link_libraries(${MAIN_APP_NAME} "-framework cocoa")
elseif(WIN32)
    target_link_libraries(${MAIN_APP_NAME} PRIVATE gdiplus)
endif()

if(UNIX)
    add_custom_target(runmain
                      COMMAND ${MAIN_APP_NAME}
                      DEPENDS ${MAIN_APP_NAME}
                      WORKING_DIRECTORY ${CMAKE_PROJECT_DIR})
endif()
                  
# Bench test app
set(TEST_BENCH test_bench)
add_executable(${TEST_BENCH} WIN32 MACOSX_BUNDLE src/test_bench.cpp)
set_property(TARGET ${TEST_BENCH} PROPERTY CXX_STANDARD 17)
target_include_directories(${TEST_BENCH}
                           PUBLIC vendor/
                           PRIVATE ${FLTK_INCLUDE_DIRS})

target_link_libraries(${TEST_BENCH}
    PRIVATE fltk fltk_forms fltk_gl fltk_images fltk_jpeg)
if(APPLE)
    target_link_libraries(${TEST_BENCH} "-framework cocoa")
elseif(WIN32)
    target_link_libraries(${TEST_BENCH} PRIVATE gdiplus)
endif()

if(UNIX)
    add_custom_target(runbench 
                      COMMAND ${TEST_BENCH}
                      DEPENDS ${TEST_BENCH}
                      WORKING_DIRECTORY ${CMAKE_PROJECT_DIR})
endif()
