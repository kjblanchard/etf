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
    src/Content.cpp
    src/Content/Sfx.cpp
)
target_compile_options(${LIBRARY_TARGET_NAME} PRIVATE ${FULL_COMPILE_OPTIONS})
target_link_directories(${LIBRARY_TARGET_NAME} PUBLIC /usr/local/share/dotnet/shared/Microsoft.NETCore.App/6.0.16/ /usr/local/share/dotnet/packs/Microsoft.NETCore.App.Host.osx-arm64/6.0.16/runtimes/osx-arm64/native/
)
target_link_libraries(${LIBRARY_TARGET_NAME}
    PUBLIC
    SupergoonEngineSound
    SupergoonEngineExternal
    nethost
)
target_include_directories(${LIBRARY_TARGET_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/external/include
    ${CMAKE_CURRENT_SOURCE_DIR}/sound/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    /usr/local/share/dotnet/packs/Microsoft.NETCore.App.Host.osx-arm64/6.0.16/runtimes/osx-arm64/native/

)
