# Variables
set(EXECUTABLE_TARGET_NAME SupergoonBlackJack)

#
cmake_minimum_required(VERSION 3.16)
message(STATUS "Starting ${EXECUTABLE_TARGET_NAME} library initialization")
add_executable(${EXECUTABLE_TARGET_NAME}
    BlackjackGame.cpp
    Entities/PlayerStart.cpp
    Systems/PlayerSystem.cpp
    Systems/ImageSystem.cpp
    Systems/LocationSystem.cpp
    Systems/AsepriteSystem.cpp
    Systems/CameraSystem.cpp
    Systems/DebugDrawSystem.cpp
)
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
