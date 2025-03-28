cmake_minimum_required(VERSION 3.22)

if(CMAKE_GENERATOR STREQUAL "Xcode")
    message(STATUS "Setting Build to Universal Binary")
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "" FORCE)
endif(CMAKE_GENERATOR STREQUAL "Xcode")

project(SupergoonEngine)

# Engine is C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Game is cp
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Should we include Imgui stuffs
option(imgui "Include ImGui support" ON)
option(SYSTEM_PACKAGES "Use system packages if available" ON)
option(ENGINE_CACHED "Is the engine cloned locally in the ./engine directory or should it be downloaded" ON)
option(BUILD_TESTING "Should we build test projects" OFF)
option(DEBUG_CMAKE_VARIABLES "Print out all debug variables near end of configure" OFF)
option(DEBUG_BUILD_TIME "Add in the pch that I think will be changing a bit" OFF)

if(DEBUG_BUILD_TIME)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")
endif(DEBUG_BUILD_TIME)

include(FetchContent)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_TARGET_NAME EscapeTheFate)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_definitions(
    -DFLECS_CUSTOM_BUILD
    -DFLECS_CPP
)

if(imgui)
    add_definitions(-Dimgui)
endif(imgui)

if(ENGINE_CACHED AND SYSTEM_PACKAGES)
    add_subdirectory(engine)
else()
    message(STATUS "Fetching supergoon engine from git..")
    FetchContent_Declare(
        engine
        GIT_REPOSITORY https://github.com/kjblanchard/sgEngine

        # Use master for development, otherwise make sure to pin to proper engine tag for releases
        GIT_TAG c55aa25

        # GIT_TAG master
    )
    FetchContent_MakeAvailable(engine)
endif(ENGINE_CACHED AND SYSTEM_PACKAGES)

add_subdirectory(src/game)

if(DEBUG_CMAKE_VARIABLES)
    get_cmake_property(ALL_VARIABLES VARIABLES)

    foreach(VAR ${ALL_VARIABLES})
        message(STATUS "${VAR} = ${${VAR}}")
    endforeach()
endif(DEBUG_CMAKE_VARIABLES)

set(FOLDERS_TO_COPY assets)

if(MSVC)
    set(INSTALL_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>)
elseif(APPLE)
    if(CMAKE_GENERATOR STREQUAL "Xcode")
        if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
            set(INSTALL_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/${EXECUTABLE_TARGET_NAME}.app)
        else()
            set(INSTALL_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/${EXECUTABLE_TARGET_NAME}.app/Contents/Resources)
        endif(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    else()
        set(INSTALL_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${EXECUTABLE_TARGET_NAME}.app/Contents/Resources)
    endif(CMAKE_GENERATOR STREQUAL "Xcode")

else()
    set(INSTALL_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif(MSVC)

install(DIRECTORY ${FOLDERS_TO_COPY}
    DESTINATION ${INSTALL_DIR}
    COMPONENT assets)
