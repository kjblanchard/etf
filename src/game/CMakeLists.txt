# Variables
cmake_minimum_required(VERSION 3.16)
message(STATUS "Starting ${EXECUTABLE_TARGET_NAME} library initialization")
set(DEBUG_FILES)

if(imgui)
    list(APPEND DEBUG_FILES
        Debug/PlayerCollider.cpp
        Debug/BattleWidget.cpp
        Systems/DebugDrawSystem.cpp
    )
endif(imgui)

if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html" CACHE STRING "Use html" FORCE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s ALLOW_MEMORY_GROWTH=1 -s STACK_SIZE=1mb -s EXPORTED_RUNTIME_METHODS=cwrap -s MODULARIZE=1 -s EXPORT_NAME='my_app' -s 'DEFAULT_LIBRARY_FUNCS_TO_INCLUDE=[\"$autoResumeAudioContext\",\"$dynCall\"]'")
elseif(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14" CACHE STRING "Minimum OS X deployment version")
    set(IS_BUNDLE "MACOSX_BUNDLE" CACHE STRING "Is this a bundle")
    set(ICON_PATH ${CMAKE_SOURCE_DIR}/cmake/AppIcon.icns CACHE STRING "Icon path")
endif(EMSCRIPTEN)

add_executable(${EXECUTABLE_TARGET_NAME}
    ${IS_BUNDLE} ${IS_WIN32}
    EscapeTheFate.cpp
    Entities/PlayerStart.cpp
    Entities/PlayerExit.cpp
    Entities/TextInteraction.cpp
    Entities/Battle/BattleZone.cpp
    Entities/Battle/BattleLocation.cpp
    Systems/PlayerSystem.cpp
    Systems/ImageSystem.cpp
    Systems/AsepriteSystem.cpp
    Systems/GameStateSystem.cpp
    Systems/CameraSystem.cpp
    Systems/TextInteractionSystem.cpp
    Systems/Battle/BattleZoneSystem.cpp
    Systems/Battle/BattleLocationSystem.cpp
    Systems/Battle/BattleSystem.cpp
    Systems/Battle/BattleUISystem.cpp
    Utilities/Utilities.cpp
    Utilities/Events.cpp
    ${DEBUG_FILES}
    ${ICON_PATH}
)

if(APPLE)
    set_target_properties(${EXECUTABLE_TARGET_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/cmake/Info.plist)
    set_source_files_properties(${ICON_PATH} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set_source_files_properties(${CMAKE_SOURCE_DIR}/cmake/Info.plist PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources
    )
elseif(EMSCRIPTEN)
    set_target_properties(${EXECUTABLE_TARGET_NAME} PROPERTIES LINK_FLAGS ${LINK_FLAGS} " -O0 -g -s --preload-file ${CMAKE_SOURCE_DIR}/assets@/assets")
endif(APPLE)

target_compile_options(${EXECUTABLE_TARGET_NAME} PRIVATE ${FULL_COMPILE_OPTIONS})
target_link_libraries(${EXECUTABLE_TARGET_NAME}
    PRIVATE
    SupergoonEngine
)
target_include_directories(${EXECUTABLE_TARGET_NAME}
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/engine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)
