# Variables
set(LIBRARY_TARGET_NAME SupergoonEngineExternal)
set(FULL_COMPILE_OPTIONS
    -Wall
    -Wextra
    -Wpedantic
    -Wdeprecated
    CACHE
    STRING
    "Compile options for everything"
)
cmake_minimum_required(VERSION 3.16)
message(STATUS "Starting ${LIBRARY_TARGET_NAME} library initialization")
add_library(${LIBRARY_TARGET_NAME} STATIC
src/imgui_impl_sdl3.cpp
src/imgui_impl_sdlrenderer3.cpp
src/imgui_demo.cpp
src/imgui_tables.cpp
src/imgui_widgets.cpp
src/imgui_draw.cpp
src/imgui.cpp
)
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
find_library(LIB_VORBIS
    REQUIRED
    PATH_SUFFIXES .a
    NAMES libvorbis vorbis vorbis.a vorbis.dll libvorbis.a
    HINTS /usr/local/lib /c/cmake/lib)
find_library(LIB_VORBISFILE
    REQUIRED
    PATH_SUFFIXES .a
    NAMES libvorbisfile vorbisfile vorbisfile.a vorbisfile.dll
    HINTS /usr/local/lib)
find_library(LIB_OGG
    REQUIRED
    PATH_SUFFIXES .a .dll
    NAMES ogg Ogg
    HINTS /usr/local/lib)
target_compile_options(${LIBRARY_TARGET_NAME} PRIVATE ${FULL_COMPILE_OPTIONS})
target_link_directories(${LIBRARY_TARGET_NAME}
    PUBLIC
    /usr/local/lib
)
target_link_libraries(${LIBRARY_TARGET_NAME}
    INTERFACE
    SDL3::SDL3-static
    vorbis vorbisenc vorbisfile ogg
)
target_include_directories(${LIBRARY_TARGET_NAME}
    PUBLIC
    /usr/local/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
