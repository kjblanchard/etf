# Subdirectories
add_subdirectory(external)
add_subdirectory(sound)

# Variables
set(LIBRARY_TARGET_NAME SupergoonEngine)

#
cmake_minimum_required(VERSION 3.16)
message(STATUS "Starting ${LIBRARY_TARGET_NAME} library initialization")
set(SRC_FILES
    src/Game.cpp
    src/Video.cpp
    src/Sound.cpp
    src/Tween.cpp
    src/Events.cpp
    src/Aseprite/AsepriteAnimation.cpp
    src/Aseprite/AsepriteDocument.cpp
    src/Content/Sfx.cpp
    src/Content/Content.cpp
    src/Content/ContentRegistry.cpp
    src/Content/Image.cpp
    src/Graphics/Graphics.cpp
    src/World/Level.cpp
    src/World/TiledMap.cpp
    src/World/World.cpp
    src/Physics/Physics.cpp
    src/Physics/RigidbodyComponent.cpp
    src/Physics/StaticSolidComponent.cpp
    src/ECS/GameObject.cpp
)

# Conditionally append the widget files if ImGui is enabled
if(imgui)
    message(STATUS "ImGui is enabled, adding ImGui-related files.")
    list(APPEND SRC_FILES
        src/Widgets/Widgets.cpp
        src/Widgets/Sound.cpp
        src/Widgets/Content.cpp
        src/Widgets/Game.cpp
    )
else()
    message(STATUS "ImGui is not enabled.")
endif()
add_library(${LIBRARY_TARGET_NAME}
    STATIC
    ${SRC_FILES}
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
