cmake_minimum_required(VERSION 3.26)

project(InkBreaker VERSION 0.1.0 LANGUAGES CXX)

# Add this line near the top of your CMakeLists.txt, after 'project()'
# This sets the CMAKE_MSVC_RUNTIME_LIBRARY variable to statically link the C++ runtime.
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:>" CACHE STRING "MSVC runtime library setting")
set_property(CACHE CMAKE_MSVC_RUNTIME_LIBRARY PROPERTY STRINGS "MultiThreaded" "MultiThreadedDebug" "MultiThreadedDLL" "MultiThreadedDebugDLL")

# You can also use this, which is a simpler approach that works for most cases
# set(CMAKE_CXX_FLAGS "/MT")
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_SHARED_LIBS OFF)
set(RESOURCE_DIR ${CMAKE_SOURCE_DIR}/resources)
set(RESOURCE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/resources)

add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${RESOURCE_DIR}
        ${RESOURCE_OUTPUT_DIR}
        COMMENT "Copying resources to build folder"
)

set(SOURCES
        src/ui/examples.h
        src/examples_2.h
        src/ui/widget.cpp
        src/ui/window.cpp
        src/ui/window.h
        src/ui/container.cpp
        src/ui/container.h
        src/ui/flex_container.cpp
        src/ui/flex_container.h
        src/ui/Center.cpp
        src/ui/Center.h
        src/ui/align.cpp
        src/ui/align.h
        src/ui/padding.cpp
        src/ui/padding.h
        src/ui/constrained_box.cpp
        src/ui/constrained_box.h
        src/ui/unconstrained_box.cpp
        src/ui/unconstrained_box.h
        src/ui/text.cpp
        src/ui/text.h
        src/graphics/geometry.h
        src/graphics/renderer.cpp
        src/graphics/renderer.h
        src/graphics/blend2d/blend2d_renderer.cpp
        src/graphics/blend2d/blend2d_renderer.h
        src/graphics/surface.cpp
        src/graphics/surface.h
        src/graphics/blend2d/blend2d_surface.cpp
        src/graphics/blend2d/blend2d_surface.h
        src/graphics/image_info.cpp
        src/graphics/image_info.h
        src/graphics/paint.cpp
        src/graphics/paint.h
        src/graphics/color.h
        src/graphics/contexts.h
        src/graphics/render.h
        src/graphics/blend2d/blend2d_utils.cpp
        src/graphics/blend2d/blend2d_utils.h
        src/graphics/font.h
        src/graphics/blend2d/blend2d_font.cpp
        src/graphics/blend2d/blend2d_font.h
        src/graphics/font.cpp
)

add_executable(inkbreaker src/main.cpp ${SOURCES})
target_include_directories(inkbreaker PRIVATE src)
# Add this flag to your target's compile options
target_compile_options(inkbreaker PRIVATE "-g -fsanitize=address -fno-omit-frame-pointer")

add_executable(workbench tests/workbench.cpp ${SOURCES})
target_include_directories(workbench PRIVATE src)

add_dependencies(inkbreaker copy_resources)

include(FetchContent)

# SDL 3
FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.2.18
)
FetchContent_MakeAvailable(SDL3)
target_link_libraries(inkbreaker PRIVATE SDL3::SDL3-static)

# Skia
set(SKIA_PATH "" CACHE PATH "Skia lib path")
if(SKIA_PATH)
    message(STATUS "Using Skia from: ${SKIA_PATH}")

    if(CMAKE_BUILD_TYPE)
        set(SKIA_BUILD_DIR "${SKIA_PATH}/out/${CMAKE_BUILD_TYPE}")
    else()
        set(SKIA_BUILD_DIR "${SKIA_PATH}/out/$<CONFIG>")
    endif()

    set(SKIA_INCLUDE_DIRS
            "${SKIA_PATH}"
            "${SKIA_PATH}/include"
            "${SKIA_PATH}/modules"
    )

    foreach(TARGET_NAME inkbreaker workbench)
        target_compile_definitions(${TARGET_NAME} PRIVATE __forceinline=inline)
        target_include_directories(${TARGET_NAME} PRIVATE ${SKIA_INCLUDE_DIRS})
        target_link_libraries(${TARGET_NAME} PRIVATE
                "${SKIA_BUILD_DIR}/skia.lib"
        )
    endforeach()
else()
    # MSYS2/Clang64 env
#    set(PKG_CONFIG_EXECUTABLE C:/msys64/clang64/bin/pkgconf.exe)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(Skia REQUIRED skia)

    target_include_directories(inkbreaker SYSTEM PRIVATE ${Skia_INCLUDE_DIRS})
    target_link_libraries(inkbreaker PRIVATE ${Skia_LIBRARIES})
endif ()

# Boost
set(Boost_INCLUDE_DIRS "C:/Users/daniel/dev/boost_1_88_0")
target_include_directories(inkbreaker PRIVATE ${Boost_INCLUDE_DIRS})

# Asmjit required by Blend2d
set(ASMJIT_DIR "${CMAKE_SOURCE_DIR}/vendor/asmjit"
        CACHE PATH "Location of 'blend2d'")

# Blend2d
set(BLEND2D_DIR "${CMAKE_SOURCE_DIR}/vendor/blend2d"
        CACHE PATH "Location of 'blend2d'")
set(BLEND2D_STATIC TRUE)
include("${BLEND2D_DIR}/CMakeLists.txt")

target_link_libraries(inkbreaker PRIVATE blend2d::blend2d)

# Executável de testes
target_link_libraries(workbench PRIVATE SDL3::SDL3)
target_include_directories(workbench PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(workbench PRIVATE blend2d::blend2d)
