# Variables
cmake_minimum_required(VERSION 3.16)
message(STATUS "Starting ${EXECUTABLE_TARGET_NAME} library initialization")

if(imgui)
    set(DEBUG_FILES
        Debug/PlayerCollider.cpp
    )
else()
    set(DEBUG_FILES)
endif(imgui)

if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html" CACHE STRING "Use html" FORCE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s 'DEFAULT_LIBRARY_FUNCS_TO_INCLUDE=[\"$autoResumeAudioContext\",\"$dynCall\"]'")
endif(EMSCRIPTEN)

if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14" CACHE STRING "Minimum OS X deployment version")
    set(IS_BUNDLE "MACOSX_BUNDLE" CACHE STRING "Is this a bundle")
    set(ICON_PATH ${CMAKE_SOURCE_DIR}/cmake/AppIcon.icns CACHE STRING "Icon path")
endif(APPLE)

add_executable(${EXECUTABLE_TARGET_NAME}
    ${IS_BUNDLE} ${IS_WIN32}
    BlackjackGame.cpp
    Entities/PlayerStart.cpp
    Systems/PlayerSystem.cpp
    Systems/ImageSystem.cpp
    Systems/AsepriteSystem.cpp
    Systems/CameraSystem.cpp
    Systems/DebugDrawSystem.cpp
    ${DEBUG_FILES}
    ${ICON_PATH}
)

if(APPLE)
    set_target_properties(${EXECUTABLE_TARGET_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/cmake/Info.plist)
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

if(APPLE)
    set_source_files_properties(${ICON_PATH} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set_source_files_properties(${CMAKE_SOURCE_DIR}/cmake/Info.plist PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources
    )
endif(APPLE)
