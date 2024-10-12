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
    src/Sound.cpp
    src/Tween.cpp
    src/Events.cpp
    src/Content/Sfx.cpp
    src/Content/Content.cpp
    src/Content/ContentRegistry.cpp
    src/Content/Image.cpp

    src/Widgets/Widgets.cpp
    src/Widgets/Sound.cpp
    src/Widgets/Content.cpp
    src/Graphics/Graphics.cpp
    src/World/Level.cpp
    src/World/TiledMap.cpp
    src/World/World.cpp

    src/ECS/GameObject.cpp

)
target_compile_options(${LIBRARY_TARGET_NAME} PRIVATE ${FULL_COMPILE_OPTIONS})
target_link_libraries(${LIBRARY_TARGET_NAME}
    PUBLIC
    SupergoonEngineSound
    SupergoonEngineExternal
)
target_include_directories(${LIBRARY_TARGET_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/external/include
    ${CMAKE_CURRENT_SOURCE_DIR}/sound/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
