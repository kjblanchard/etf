# Variables
set(LIBRARY_TARGET_NAME SupergoonEngineExternal)

set(FULL_COMPILE_OPTIONS
    # -Wall
    # -Wextra
    # -Wpedantic
    # -Wdeprecated
    # -Wno-deprecated-literal-operator
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
    find_package(SDL3 CONFIG COMPONENTS SDL3-shared)
    find_package(PNG )
    find_library(LIB_VORBIS

        PATH_SUFFIXES .a
        NAMES libvorbis vorbis vorbis.a vorbis.dll libvorbis.a
        HINTS /usr/local/lib /c/cmake/lib)

    find_library(LIB_VORBISFILE

        PATH_SUFFIXES .a
        NAMES libvorbisfile vorbisfile vorbisfile.a vorbisfile.dll
        HINTS /usr/local/lib)
    find_library(LIB_OGG

        PATH_SUFFIXES .a .dll
        NAMES ogg Ogg
        HINTS /usr/local/lib)
endif(SYSTEM_PACKAGES)

set(SKIP_INSTALL_ALL ON CACHE BOOL "Don't install this")

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
        EXCLUDE_FROM_ALL
    )

    # Make sure to fetch the content
    FetchContent_MakeAvailable(SDL3)
    # Make sure to fetch the content
endif(NOT SDL3_FOUND)

if(NOT ZLIB_FOUND)
    message(STATUS "ZLIB not found. Fetching zlib...")
    FetchContent_Declare(
        zlib
        GIT_REPOSITORY https://github.com/madler/zlib
        GIT_TAG v1.3.1
        EXCLUDE_FROM_ALL
    )
    include_directories(${zlib_SOURCE_DIR})
    include_directories(${zlib_BINARY_DIR})
    # FetchContent_GetProperties(zlib)
    # if(NOT zlib_POPULATED)
    #     FetchContent_Populate(zlib)
    #     add_subdirectory(${zlib_SOURCE_DIR} ${zlib_BINARY_DIR})
    # endif()
    FetchContent_MakeAvailable(zlib)
endif(NOT PNG_FOUND)

get_cmake_property(ALL_VARIABLES VARIABLES)
foreach(VAR ${ALL_VARIABLES})
    message(STATUS "${VAR} = ${${VAR}}")
endforeach()

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
    # get_target_property(OGG_LIBRARY ogg IMPORTED_LOCATION)

#     if(NOT OGG_LIBRARY)
#     # Print all CMake variables

#         message(FATAL_ERROR "OGG_LIBRARY could not be found!")
#     endif()
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
    # set(PC_OGG_INCLUDEDIR ${CMAKE_BINARY_DIR}/_deps/lib_ogg-src/include CACHE STRING "YES" FORCE)
    # set(OGG_INCLUDE_DIR ${LIB_OGG_SOURCE_DIR}/include CACHE STRING "Include directory for OGG")
    # set(PC_OGG_LIBDIR ${CMAKE_BINARY_DIR}/_deps/lib_ogg-build CACHE STRING "YES" FORCE)
    FetchContent_MakeAvailable(LIB_VORBIS)
endif(NOT LIB_VORBIS)

target_compile_options(${LIBRARY_TARGET_NAME} PRIVATE ${FULL_COMPILE_OPTIONS})
target_link_directories(${LIBRARY_TARGET_NAME}
    PUBLIC /usr/local/lib

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
    vorbisenc vorbisfile vorbis ogg
)
target_include_directories(${LIBRARY_TARGET_NAME}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_BINARY_DIR}/_deps/sdl3-src/include
    ${CMAKE_INSTALL_INCLUDEDIR}
    /usr/local/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
