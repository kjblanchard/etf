# Variables
set(LIBRARY_TARGET_NAME SupergoonEngineExternal)
set(COMPILE_OPTIONS )
if(UNIX OR APPLE)
list ( APPEND COMPILE_OPTIONS
    -Wall
    -Wextra
    -Wpedantic
    -Wdeprecated
    -Wno-deprecated-literal-operator
)
endif(UNIX OR APPLE)
message(STATUS "Starting ${LIBRARY_TARGET_NAME} library initialization")
cmake_minimum_required(VERSION 3.24)
set(ZLIB_USE_STATIC_LIBS "ON") #We should always link to static zlib
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
set(SKIP_INSTALL_ALL ON CACHE BOOL "We don't want any of these external packages to install anything")
set(CMAKE_DEBUG_POSTFIX "" CACHE STRING " Don't use the d for debug packages" FORCE)
set(CMAKE_DEBUG_TARGET_PROPERTIES "DEBUG_OUTPUT" "" CACHE STRING "Don't use the d for debug packages" FORCE)

if(SYSTEM_PACKAGES) #If we are using system packages, then we should try to find them.
    find_package(SDL3 CONFIG COMPONENTS SDL3-shared)
    find_package(PNG )
    find_library(LIB_VORBIS
        PATH_SUFFIXES .a .dll
        NAMES libvorbis vorbis vorbis.a vorbis.dll libvorbis.a
        HINTS /usr/local/lib /c/cmake/lib)

    find_library(LIB_VORBISFILE
        PATH_SUFFIXES .a .dll
        NAMES libvorbisfile vorbisfile vorbisfile.a vorbisfile.dll
        HINTS /usr/local/lib)
    find_library(LIB_OGG

        PATH_SUFFIXES .a .dll
        NAMES ogg Ogg
            HINTS /usr/local/lib)
        find_package(ZLIB)
endif(SYSTEM_PACKAGES)


if(NOT SDL3_FOUND)
    message(STATUS "SDL3 not found. Fetching SDL3...")
    set(SDL_SHARED OFF CACHE BOOL "Build SDL as a shared library")
    set(SDL_STATIC ON CACHE BOOL "Build SDL as a shared library")
    set(SDL_TEST OFF CACHE BOOL "Build SDL as a shared library")
    set(ENABLE_SDL_STATIC OFF CACHE BOOL "Build SDL as a static library")
    FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL
        GIT_TAG main
        EXCLUDE_FROM_ALL
    )
    FetchContent_MakeAvailable(SDL3)
endif(NOT SDL3_FOUND)


if(NOT ZLIB_FOUND)
    message(STATUS "ZLIB not found. Fetching zlib...")
    set(ZLIB_BUILD_EXAMPLES OFF CACHE BOOL "Do not build zlib examples" FORCE)
    set(RENAME_ZCONF OFF CACHE BOOL "Do not build zlib examples" FORCE)
    FetchContent_Declare(
        zlib
        GIT_REPOSITORY https://github.com/madler/zlib
        GIT_TAG v1.3.1
        EXCLUDE_FROM_ALL
    )
    # include_directories(${zlib_SOURCE_DIR})
    # include_directories(${zlib_BINARY_DIR})
    FetchContent_GetProperties(zlib)
    if(NOT zlib_POPULATED)
        FetchContent_Populate(zlib)
        add_subdirectory(${zlib_SOURCE_DIR} ${zlib_BINARY_DIR})
    endif()
endif(NOT ZLIB_FOUND)


if(NOT PNG_FOUND)
    set(PNG_TESTS OFF CACHE BOOL "Build the libpng tests" FORCE)
    set(PNG_BUILD_ZLIB OFF  CACHE BOOL "Always build ZLIB if needed" FORCE)
    set(PNG_TOOLS OFF CACHE BOOL "Build the libpng tests" FORCE)
    set(ZLIB_LIBRARY zlib)
    set(ZLIB_ROOT ${zlib_SOURCE_DIR} CACHE STRING "PATH TO SUBDIR ZLIB" FORCE)
    set(PNG_SHARED OFF CACHE BOOL "Build the libpng tests" FORCE)
    message(STATUS "PNG not found. Fetching PNG...")
    FetchContent_Declare(
        PNG
        GIT_REPOSITORY https://github.com/pnggroup/libpng.git
        GIT_TAG v1.6.44
        EXCLUDE_FROM_ALL
    )
    FetchContent_MakeAvailable(PNG)
endif(NOT PNG_FOUND)


if(NOT LIB_OGG)
    message(STATUS "OGG not found. Fetching OGG...")
    FetchContent_Declare(
        LIB_OGG
        GIT_REPOSITORY https://github.com/xiph/ogg
        GIT_TAG v1.3.5
        EXCLUDE_FROM_ALL
    )
    FetchContent_MakeAvailable(LIB_OGG)
        get_target_property(OGG_INCLUDE_DIR ogg INTERFACE_INCLUDE_DIRECTORIES)
        set(OGG_LIBRARY ogg)
endif(NOT LIB_OGG)

if(NOT LIB_VORBIS)
    message(STATUS "VORBIS not found. Fetching VORBIS...")
    set(INSTALL_CMAKE_PACKAGE_MODULE OFF CACHE BOOL "Dont do it" FORCE)
    FetchContent_Declare(
        LIB_VORBIS
        GIT_REPOSITORY https://github.com/xiph/vorbis
        GIT_TAG v1.3.7
        EXCLUDE_FROM_ALL
    )
    FetchContent_MakeAvailable(LIB_VORBIS)
endif(NOT LIB_VORBIS)

# get_cmake_property(ALL_VARIABLES VARIABLES)
# foreach(VAR ${ALL_VARIABLES})
#     message(STATUS "${VAR} = ${${VAR}}")
# endforeach()

target_compile_options(${LIBRARY_TARGET_NAME} PUBLIC ${COMPILE_OPTIONS})
target_link_directories(${LIBRARY_TARGET_NAME} PUBLIC /usr/local/lib) #should always include from usr/local/lib

target_link_libraries(${LIBRARY_TARGET_NAME}
    INTERFACE
    SDL3::SDL3-static
    ${ZLIB_LIBRARIES}
    png_static
    vorbisenc vorbisfile vorbis ogg
)
target_include_directories(${LIBRARY_TARGET_NAME}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
    # If we had to build from subdirectories, include their things needed.
    # ${CMAKE_INSTALL_INCLUDEDIR}
    /usr/local/include
    PRIVATE
    ${sdl3_SOURCE_DIR}/include
    ${png_SOURCE_DIR}
    ${png_BINARY_DIR}
    ${zlib_SOURCE_DIR}
    ${zlib_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
