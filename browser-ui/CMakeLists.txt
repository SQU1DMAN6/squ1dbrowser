cmake_minimum_required(VERSION 3.20)
project(squ1d-browser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Skia
find_package(skia QUIET)
if(NOT skia_FOUND)
    message(STATUS "Skia not found, will be built or downloaded")
    # You can add logic here to download/build Skia if needed
endif()

# SDL2 for windowing
find_package(SDL2 REQUIRED)

# Main browser executable
add_executable(squ1d-browser
    src/main.cpp
    src/browser_window.cpp
    src/ui_renderer.cpp
    src/renderer_bridge.cpp
    src/tab_manager.cpp
)

target_include_directories(squ1d-browser PRIVATE
    src
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(squ1d-browser PRIVATE
    ${SDL2_LIBRARIES}
)

# If Skia is available
if(skia_FOUND)
    target_link_libraries(squ1d-browser PRIVATE skia::skia)
endif()

# Detect if Skia needs to be built
if(NOT skia_FOUND)
    message(WARNING "Skia not available. Using Skia system installation or manual setup required.")
endif()
