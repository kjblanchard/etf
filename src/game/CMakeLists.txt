# Variables
set(EXECUTABLE_TARGET_NAME SupergoonBlackJack)
cmake_minimum_required(VERSION 3.16)
message(STATUS "Starting ${EXECUTABLE_TARGET_NAME} library initialization")

if(imgui)
    set(DEBUG_FILES
        Debug/PlayerCollider.cpp
    )
else()
    set(DEBUG_FILES)
endif(imgui)

add_executable(${EXECUTABLE_TARGET_NAME}
    BlackjackGame.cpp
    Entities/PlayerStart.cpp
    Systems/PlayerSystem.cpp
    Systems/ImageSystem.cpp
    Systems/AsepriteSystem.cpp
    Systems/CameraSystem.cpp
    Systems/DebugDrawSystem.cpp
    ${DEBUG_FILES}
)
set(CMAKE_EXECUTABLE_SUFFIX ".html" CACHE STRING "Use html" FORCE)
set_target_properties(${EXECUTABLE_TARGET_NAME} PROPERTIES LINK_FLAGS ${LINK_FLAGS} " -O0 -g -s --preload-file ${CMAKE_SOURCE_DIR}/assets@/assets")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s 'DEFAULT_LIBRARY_FUNCS_TO_INCLUDE=[\"$autoResumeAudioContext\",\"$dynCall\"]'")
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
