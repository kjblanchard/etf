# Variables
set(LIBRARY_TARGET_NAME SupergoonEngineSound)
cmake_minimum_required(VERSION 3.16)
message(STATUS "Starting ${LIBRARY_TARGET_NAME} library initialization")
add_library(${LIBRARY_TARGET_NAME} STATIC src/Bgm.c)
target_compile_options(${LIBRARY_TARGET_NAME} PRIVATE ${FULL_COMPILE_OPTIONS})
target_link_libraries(${LIBRARY_TARGET_NAME}
    PRIVATE
    SupergoonEngineExternal
)
target_include_directories(${LIBRARY_TARGET_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/engine/external/include
    ${CMAKE_CURRENT_}
)