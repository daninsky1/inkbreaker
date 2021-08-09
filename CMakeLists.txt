cmake_minimum_required(VERSION 3.16)

project(inkbreaker
        VERSION 0.1.0
        LANGUAGES C CXX)


# toolchain var: -DCMAKE_TOOLCHAIN_FILE=[toolchain.cmake path]
# TODO: add MSVC fltk dir resolution
if(MSVC)
    add_compile_options(-W4)
elseif(MINGW)
    add_compile_options(-mwindows -lgdiplus)
    set(FLTK_DIR "vendor/fltk/mingw/")
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
    set(FLTK_DIR "vendor/fltk/build/")
endif()

# Find fltk package
find_package(FLTK REQUIRED NO_MODULE)

# Main app
set(MAIN_APP InkBreaker)
add_executable(${MAIN_APP} WIN32 MACOSX_BUNDLE src/main.cpp)
set_property(TARGET ${MAIN_APP} PROPERTY CXX_STANDARD 17)
target_include_directories(${MAIN_APP}
                           PRIVATE ${FLTK_INCLUDE_DIRS})

target_link_libraries(${MAIN_APP}
    PRIVATE fltk fltk_forms fltk_gl fltk_images fltk_jpeg)
if(APPLE)
    target_link_libraries(${MAIN_APP} "-framework cocoa")
elseif(WIN32)
    target_link_libraries(${MAIN_APP} PRIVATE gdiplus)
endif()

add_custom_target(runmain
                  COMMAND ${MAIN_APP}
                  DEPENDS ${MAIN_APP}
                  WORKING_DIRECTORY ${CMAKE_PROJECT_DIR})
                  
# Bench test app
set(TEST_BENCH test_bench)
add_executable(${TEST_BENCH} WIN32 MACOSX_BUNDLE src/test_bench.cpp)
set_property(TARGET ${TEST_BENCH} PROPERTY CXX_STANDARD 17)
target_include_directories(${TEST_BENCH}
                           PRIVATE ${FLTK_INCLUDE_DIRS})

target_link_libraries(${TEST_BENCH}
    PRIVATE fltk fltk_forms fltk_gl fltk_images fltk_jpeg)
if(APPLE)
    target_link_libraries(${TEST_BENCH} "-framework cocoa")
elseif(WIN32)
    target_link_libraries(${TEST_BENCH} PRIVATE gdiplus)
endif()

add_custom_target(runbench 
                  COMMAND ${TEST_BENCH}
                  DEPENDS ${TEST_BENCH}
                  WORKING_DIRECTORY ${CMAKE_PROJECT_DIR})