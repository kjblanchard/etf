# Subdirectories
add_subdirectory(external)
add_subdirectory(sound)

# Variables
set(LIBRARY_TARGET_NAME SupergoonEngine)

#
cmake_minimum_required(VERSION 3.16)
message(STATUS "Starting ${LIBRARY_TARGET_NAME} library initialization")
add_library(${LIBRARY_TARGET_NAME}
    STATIC
    src/Game.cpp
    src/Video.cpp
)
target_compile_options(${LIBRARY_TARGET_NAME} PRIVATE ${FULL_COMPILE_OPTIONS})
target_link_libraries(${LIBRARY_TARGET_NAME}
    PUBLIC
    SupergoonEngineSound
    SupergoonEngineExternal
)
target_include_directories(${LIBRARY_TARGET_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/external/include
    ${CMAKE_CURRENT_SOURCE_DIR}/sound/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
