# Subdirectories
add_subdirectory(external)
add_subdirectory(sound)
set(LIBRARY_TARGET_NAME SupergoonEngine)
cmake_minimum_required(VERSION 3.22)
message(STATUS "Starting ${LIBRARY_TARGET_NAME} library initialization")
set(SRC_FILES
    src/Game.cpp
    src/Video.cpp
    src/Sound.cpp
    src/Tween/Tween.cpp
    src/Tween/Sequence.cpp
    src/Events.cpp
    src/Aseprite/AsepriteAnimation.cpp
    src/Content/AsepriteDocument.cpp
    src/Content/Font.cpp
    src/Content/Text.cpp
    src/Content/Sfx.cpp
    src/Content/Content.cpp
    src/Content/ContentRegistry.cpp
    src/Content/Image.cpp
    src/Graphics/Graphics.cpp
    src/World/Level.cpp
    src/World/TiledMap.cpp
    src/World/World.cpp
    src/Physics/Physics.cpp
    src/ECS/GameObject.cpp
    src/UI/Panel.cpp
    src/UI/UI.cpp
    src/UI/ImageObject.cpp
    src/UI/UIObjectAnimator.cpp
)

if(imgui)
    message(STATUS "ImGui is enabled, adding ImGui-related files.")
    list(APPEND SRC_FILES
        src/Widgets/Widgets.cpp
        src/Widgets/SoundWidget.cpp
        src/Widgets/ContentWidget.cpp
        src/Widgets/Game.cpp
        src/Widgets/UIWidget.cpp
        src/Widgets/Global.cpp
        src/Widgets/LevelWidget.cpp
    )
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
