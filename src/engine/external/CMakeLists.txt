# Variables
set(LIBRARY_TARGET_NAME SupergoonEngineExternal)
set(FULL_COMPILE_OPTIONS
    -Wall
    -Wextra
    -Wpedantic
    -Wdeprecated
    -Wno-deprecated-literal-operator
    CACHE STRING "Compile options for everything"
)
cmake_minimum_required(VERSION 3.16)
message(STATUS "Starting ${LIBRARY_TARGET_NAME} library initialization")
set(SRC_FILES
    src/easing.c
    src/log.c
    src/clock.c
    src/input/joystick.c
    src/input/keyboard.c
)

if(imgui)
    message(STATUS "ImGui is enabled, adding ImGui-related files.")
    list(APPEND SRC_FILES
        src/imgui/imgui_impl_sdl3.cpp
        src/imgui/imgui_impl_sdlrenderer3.cpp
        src/imgui/imgui_demo.cpp
        src/imgui/imgui_tables.cpp
        src/imgui/imgui_widgets.cpp
        src/imgui/imgui_draw.cpp
        src/imgui/imgui.cpp
    )
endif()

add_library(${LIBRARY_TARGET_NAME} STATIC
    ${SRC_FILES}
)

if(SYSTEM_PACKAGES)
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
    find_package(PNG REQUIRED)
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
endif(SYSTEM_PACKAGES)

if(NOT SDL3_FOUND)
    # SDL options
    set(SDL_SHARED OFF CACHE BOOL "Build SDL as a shared library")
    set(SDL_STATIC ON CACHE BOOL "Build SDL as a shared library")
    set(SDL_TEST OFF CACHE BOOL "Build SDL as a shared library")
    set(ENABLE_SDL_STATIC OFF CACHE BOOL "Build SDL as a static library")
    message(STATUS "SDL3 not found. Fetching SDL3...")
    FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL
        GIT_TAG main
    )

    # Make sure to fetch the content
    FetchContent_MakeAvailable(SDL3)
endif(NOT SDL3_FOUND)

if(NOT PNG_FOUND)
    set(PNG_TESTS OFF CACHE BOOL "Build the libpng tests" FORCE)
    set(PNG_BUILD_ZLIB OFF CACHE BOOL "Always build ZLIB if needed" FORCE)
    set(PNG_TOOLS OFF CACHE BOOL "Build the libpng tests" FORCE)
    set(PNG_SHARED OFF CACHE BOOL "Build the libpng tests" FORCE)
    set(SKIP_INSTALL_ALL ON CACHE BOOL "Don't install this")
    FetchContent_Declare(
        PNG
        GIT_REPOSITORY https://github.com/pnggroup/libpng.git
        GIT_TAG v1.6.44
    )
    FetchContent_MakeAvailable(PNG)
endif(NOT PNG_FOUND)

if(NOT LIB_VORBIS)
    set(INSTALL_CMAKE_PACKAGE_MODULE OFF CACHE BOOL "Dont do it" FORCE)
    FetchContent_Declare(
        LIB_VORBIS
        GIT_REPOSITORY https://github.com/xiph/vorbis
        GIT_TAG v1.3.7
        EXCLUDE_FROM_ALL
    )
    FetchContent_MakeAvailable(LIB_VORBIS)
endif(NOT LIB_VORBIS)

if(NOT LIB_OGG)
    FetchContent_Declare(
        LIB_OGG
        GIT_REPOSITORY https://github.com/xiph/ogg
        GIT_TAG v1.3.5
        EXCLUDE_FROM_ALL
    )
    FetchContent_MakeAvailable(LIB_OGG)
endif(NOT LIB_OGG)

target_compile_options(${LIBRARY_TARGET_NAME} PRIVATE ${FULL_COMPILE_OPTIONS})
target_link_directories(${LIBRARY_TARGET_NAME}
    PUBLIC
    /usr/local/lib
)

if(WIN32 OR(MACOS AND GOON_FULL_MACOS_BUILD))
    set(PNG_LIB_NAME png_static)
else()
    set(PNG_LIB_NAME png)
endif(WIN32 OR(MACOS AND GOON_FULL_MACOS_BUILD))

target_link_libraries(${LIBRARY_TARGET_NAME}
    INTERFACE
    SDL3::SDL3-static
    ${PNG_LIB_NAME}
    vorbis vorbisenc vorbisfile ogg
)
target_include_directories(${LIBRARY_TARGET_NAME}
    PUBLIC
    /usr/local/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
