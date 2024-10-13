cmake_minimum_required(VERSION 3.15)
project(SupergoonEngine)

# Engine is C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Game is cp
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Should we include Imgui stuffs
set(imgui OFF)

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(INSTALL_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} CACHE STRING "Where the files will be copied to." FORCE)

option(BUILD_ENGINE "Build the client" ON)
option(BUILD_GAME "Build the server" ON)

if(imgui)
add_definitions(-Dimgui)

endif(imgui)
if(BUILD_ENGINE)
    add_subdirectory(src/engine)
endif()

if(BUILD_GAME)
    add_subdirectory(src/game)
endif()

set(FOLDERS_TO_COPY assets)
install(DIRECTORY ${FOLDERS_TO_COPY}
    DESTINATION ${INSTALL_DIR}
    COMPONENT assets)
