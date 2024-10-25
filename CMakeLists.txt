cmake_minimum_required(VERSION 3.15)
project(SupergoonEngine)

# Engine is C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Game is cp
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Should we include Imgui stuffs
option(imgui "Include ImGui support" ON)
option(SYSTEM_PACKAGES "Use system packages if available" OFF)

include(FetchContent)

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(INSTALL_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} CACHE STRING "Where the files will be copied to." FORCE)

if(imgui)
    add_definitions(-Dimgui)
endif(imgui)

add_subdirectory(src/engine)
add_subdirectory(src/game)

set(FOLDERS_TO_COPY assets)
install(DIRECTORY ${FOLDERS_TO_COPY}
    DESTINATION ${INSTALL_DIR}
    COMPONENT assets)
