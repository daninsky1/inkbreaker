cmake_minimum_required(VERSION 3.16)

project(inkbreaker
        VERSION 0.1.0
        LANGUAGES C CXX)

add_compile_options(-Wall -Wextra -Wpedantic)

# Find fltk package
set(FLTK_DIR "vendor/fltk/build/")
find_package(FLTK REQUIRED NO_MODULE)

# Main app name
set(MAIN_APP InkBreaker)
add_executable(${MAIN_APP} WIN32 MACOSX_BUNDLE src/main.cpp)
set_property(TARGET ${MAIN_APP} PROPERTY CXX_STANDARD 17)
target_include_directories(${MAIN_APP}
                           PUBLIC ${FLTK_INCLUDE_DIRS})

target_link_libraries(${MAIN_APP}
    PUBLIC fltk fltk_forms fltk_gl fltk_images fltk_jpeg)
if(APPLE)
    target_link_libraries(${MAIN_APP} "-framework cocoa")
endif(APPLE)

add_custom_target(run 
                  COMMAND ${MAIN_APP}
                  DEPENDS ${MAIN_APP}
                  WORKING_DIRECTORY ${CMAKE_PROJECT_DIR})
                  
# Main app name
set(TEST_BENCH test_bench)
add_executable(${TEST_BENCH} WIN32 MACOSX_BUNDLE src/test_bench.cpp)
set_property(TARGET ${TEST_BENCH} PROPERTY CXX_STANDARD 17)
target_include_directories(${TEST_BENCH}
                           PUBLIC ${FLTK_INCLUDE_DIRS})

target_link_libraries(${TEST_BENCH}
    PUBLIC fltk fltk_forms fltk_gl fltk_images fltk_jpeg)
if(APPLE)
    target_link_libraries(${TEST_BENCH} "-framework cocoa")
endif(APPLE)

add_custom_target(runbench 
                  COMMAND ${TEST_BENCH}
                  DEPENDS ${TEST_BENCH}
                  WORKING_DIRECTORY ${CMAKE_PROJECT_DIR})