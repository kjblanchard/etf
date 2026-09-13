# 3.26 is needed for building al on static
cmake_minimum_required(VERSION 3.28)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_STANDARD 23)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(GAME_VERSION "0.1.0" CACHE STRING "Game version string")
set(STEAM_APP_ID 4393160)
project("Escape The Fate" LANGUAGES C CXX VERSION ${GAME_VERSION})
option(SYSTEM_PACKAGES "Use system packages if available" ON)
option(ENGINE_CACHED "Is the engine cloned locally in the ./engine directory or should it be downloaded" ON)
option(DEBUG_CMAKE_VARIABLES "Print out all debug variables near end of configure" OFF)
option(LINK_M "Should we link against the M, only useful on linux builds locally")
option(IMGUI_DEBUGGING "Should we build with imgui enabled" OFF)
# option(PRELOAD_ALL_ASSETS "Preload everything at beginning, useful for EMSCRIPTEN" ON)
# If ON, gives a debug message when building about vm space or something, nanomalloc. If ON, Memory shows super high
# usage.. it's wild, to get an accurate reading switch to off.
option(DEBUG_ASAN "Use asan for leak and address sanitization checks when debug mode" ON)
option(SDL_BACKEND "Does this use SDL backend?" ON)
option(SDL_GL "Does this use SDL GL backend?" ON)
option(SDL_RENDERER "Does this use SDL renderer backend?" OFF)
option(STEAM_ENABLED "Does this use steam as a platform?" OFF)
option(STEAM_APPID_FILE "Write steam_appid.txt next to binary (dev only)" ON)
include(FetchContent)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_BINARY_DIR}/lib/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib/Release)

set(EXECUTABLE_TARGET_NAME EscapeTheFate)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INSTALL_MESSAGE LAZY)

set(EXECUTABLE_SOURCE_FILES_CORE
  src/main.c
)

if(IMGUI_DEBUGGING)
  set(IMGUI_SRC 
    external/imgui.cpp
    external/imgui_demo.cpp
    external/imgui_draw.cpp
    external/imgui_impl_opengl3.cpp
    external/imgui_tables.cpp
    external/imgui_impl_sdl3.cpp
    external/imgui_widgets.cpp
  )
  list(APPEND EXECUTABLE_SOURCE_FILES_CORE ${IMGUI_SRC})
  add_definitions(-Dimgui)
  #Ignore source file warnings from external imgui  
  set_source_files_properties( ${IMGUI_SRC} PROPERTIES COMPILE_FLAGS "-w" )
endif(IMGUI_DEBUGGING)
set(ASSET_FILE_LOCATION etf.sg)

if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET
        "10.14"
        CACHE STRING "Minimum OS X deployment version")
    set(IS_BUNDLE
        "MACOSX_BUNDLE"
        CACHE STRING "Is this a bundle")
    set(ICON_PATH
        ${CMAKE_SOURCE_DIR}/cmake/AppIcon.icns
        CACHE STRING "Icon path")
endif(APPLE)

add_executable(${EXECUTABLE_TARGET_NAME} MACOSX_BUNDLE WIN32 
  ${EXECUTABLE_SOURCE_FILES_CORE}
  ${ICON_PATH}
  ${ASSET_FILE_LOCATION}
  cmake/AppIcon.rc
)

if(MSVC)
  # set(INSTALL_DIR
elseif(APPLE)
  if(CMAKE_GENERATOR STREQUAL "Xcode")
    message(STATUS "Setting Build to Universal Binary")
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "" FORCE)
    set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${CMAKE_SOURCE_DIR}/cmake/EscapeTheFate.entitlements")
    set(XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${CMAKE_SOURCE_DIR}/cmake/EscapeTheFate.entitlements")
    set_target_properties(
      ${EXECUTABLE_TARGET_NAME}
      PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${CMAKE_SOURCE_DIR}/cmake/EscapeTheFate.entitlements"
      # XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Automatic"
      XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Manual"
      XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "16.3"
      XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "H34NGW287W"
      # when building on github, we don't want to sign anything cause there is no cert
      XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO"
      XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "NO"
      # XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Apple Development"
    )
    #Needed for codesigning and cpack
    set(CMAKE_SKIP_RPATH TRUE)
    if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
      add_definitions(-DIS_MOBILE)
      # set(INSTALL_DIR
    else()
    endif(CMAKE_SYSTEM_NAME STREQUAL "iOS")
  else()
  endif(CMAKE_GENERATOR STREQUAL "Xcode")
endif(MSVC)

if(APPLE)
  set_target_properties(${EXECUTABLE_TARGET_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/cmake/Info.plist)
  set_source_files_properties(${ICON_PATH} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  set_source_files_properties(${CMAKE_SOURCE_DIR}/cmake/Info.plist PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  set_source_files_properties(etf.sg PROPERTIES MACOSX_PACKAGE_LOCATION Resources/data)
endif(APPLE)

if(ENGINE_CACHED STREQUAL "ON")
  add_subdirectory(SupergoonEngine)
else()
  message(STATUS "Fetching supergoon engine from git..")
  FetchContent_Declare(
    SupergoonEngine
    GIT_REPOSITORY https://github.com/kjblanchard/SupergoonEngine
    # Use master for development, otherwise make sure to pin to proper engine tag for releases GIT_TAG c55aa25 GIT_TAG
    GIT_TAG lua
    #GIT_TAG master
  )
  FetchContent_MakeAvailable(SupergoonEngine)
endif(ENGINE_CACHED STREQUAL "ON")

target_include_directories(${EXECUTABLE_TARGET_NAME} PRIVATE 
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/external
  ${CMAKE_SOURCE_DIR}/SupergoonEngine/include
)

if(DEBUG_ASAN AND CMAKE_BUILD_TYPE STREQUAL "Debug")
  if(NOT (APPLE AND (CMAKE_SYSTEM_NAME STREQUAL "iOS" OR CMAKE_SYSTEM_NAME STREQUAL "iossimulator")) AND NOT (CMAKE_SYSTEM_NAME STREQUAL "Emscripten"))
    message(STATUS "Enabling AddressSanitizer in Debug mode")
    set(ASAN_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ASAN_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ASAN_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ASAN_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${ASAN_FLAGS}")
  endif()
endif()

target_link_libraries(${EXECUTABLE_TARGET_NAME} PRIVATE SupergoonEngine)

###### Platform specific executable flags
if(EMSCRIPTEN)
  message(STATUS "Enabling Emscripten flags for supergoon engine ")
  set(CMAKE_EXECUTABLE_SUFFIX
    ".html"
    CACHE STRING "Use html" FORCE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_SDL=3 -s USE_ZLIB=1  -s USE_VORBIS=1 -s USE_OGG=1 ")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=3 -s USE_ZLIB=1  -s USE_VORBIS=1 -s USE_OGG=1 ")
  set(LINK_FLAGS
    "${LINK_FLAGS} -s ALLOW_MEMORY_GROWTH=1 -s USE_VORBIS=1 -O2 -s USE_OGG=1 -s FULL_ES3=1 -s USE_WEBGL2=1 -s STACK_SIZE=1mb -s EXPORTED_RUNTIME_METHODS=cwrap -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s 'DEFAULT_LIBRARY_FUNCS_TO_INCLUDE=[\"$autoResumeAudioContext\",\"$dynCall\"]'")
  set_target_properties( ${EXECUTABLE_TARGET_NAME} PROPERTIES LINK_FLAGS
    "${LINK_FLAGS} -O0 -s SAFE_HEAP=2 -s STACK_OVERFLOW_CHECK=1 \
    --preload-file ${CMAKE_SOURCE_DIR}/etf.sg@/data/etf.sg \
    --shell-file=${CMAKE_SOURCE_DIR}/cmake/shell.html")
  elseif(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET
      "10.14"
      CACHE STRING "Minimum OS X deployment version")
    set(IS_BUNDLE
      "MACOSX_BUNDLE"
      CACHE STRING "Is this a bundle")
    set(ICON_PATH
      ${CMAKE_SOURCE_DIR}/cmake/AppIcon.icns
      CACHE STRING "Icon path")
    # target_link_options(EscapeTheFate PRIVATE "")
endif(EMSCRIPTEN)

if(STEAM_ENABLED)
    if(STEAM_APPID_FILE)
        file(WRITE "${CMAKE_BINARY_DIR}/steam_appid.txt" "${STEAM_APP_ID}\n")
    endif()
    if(APPLE)
        set(_STEAM_COPY_CMDS
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${STEAM_RUNTIME_FILES}"
                "$<TARGET_BUNDLE_CONTENT_DIR:${EXECUTABLE_TARGET_NAME}>/MacOS/libsteam_api.dylib"
        )
        if(STEAM_APPID_FILE)
            list(APPEND _STEAM_COPY_CMDS
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${CMAKE_BINARY_DIR}/steam_appid.txt"
                    "$<TARGET_BUNDLE_CONTENT_DIR:${EXECUTABLE_TARGET_NAME}>/MacOS/steam_appid.txt"
            )
        endif()
        add_custom_command(TARGET ${EXECUTABLE_TARGET_NAME} POST_BUILD
            ${_STEAM_COPY_CMDS}
            COMMAND_EXPAND_LISTS)
    elseif(WIN32)
        set(_STEAM_COPY_CMDS
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${STEAM_RUNTIME_FILES}"
                "$<TARGET_FILE_DIR:${EXECUTABLE_TARGET_NAME}>/steam_api64.dll"
        )
        if(STEAM_APPID_FILE)
            list(APPEND _STEAM_COPY_CMDS
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${CMAKE_BINARY_DIR}/steam_appid.txt"
                    "$<TARGET_FILE_DIR:${EXECUTABLE_TARGET_NAME}>/steam_appid.txt"
            )
        endif()
        add_custom_command(TARGET ${EXECUTABLE_TARGET_NAME} POST_BUILD
            ${_STEAM_COPY_CMDS}
            COMMAND_EXPAND_LISTS)
    else()
        set(_STEAM_COPY_CMDS
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${STEAM_RUNTIME_FILES}"
                "$<TARGET_FILE_DIR:${EXECUTABLE_TARGET_NAME}>/libsteam_api.so"
        )
        if(STEAM_APPID_FILE)
            list(APPEND _STEAM_COPY_CMDS
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${CMAKE_BINARY_DIR}/steam_appid.txt"
                    "$<TARGET_FILE_DIR:${EXECUTABLE_TARGET_NAME}>/steam_appid.txt"
            )
        endif()
        add_custom_command(TARGET ${EXECUTABLE_TARGET_NAME} POST_BUILD
            ${_STEAM_COPY_CMDS}
            COMMAND_EXPAND_LISTS)
    endif()
endif()

###### Packaging
set(CPACK_PACKAGE_NAME "EscapeTheFate")
set(CPACK_PACKAGE_VENDOR "Supergoon Games")
set(CPACK_PACKAGE_VERSION "${GAME_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "2D deckbuilding JRPG")
set(CPACK_NSIS_DISPLAY_NAME "Escape the Fate")
set(CPACK_NSIS_PACKAGE_NAME "Escape the Fate")
set(CPACK_NSIS_EXECUTABLES_DIRECTORY ".")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_NSIS_CONTACT "kevin@supergoon.com")
set(CPACK_NSIS_HELP_LINK "https://supergoon.com")
set(CPACK_NSIS_URL_INFO_ABOUT "https://escapethefate-dev.supergoon.com")
set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/cmake/AppIcon.ico")
set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/cmake/AppIcon.ico")
set(CPACK_PACKAGE_EXECUTABLES "EscapeTheFate" "Escape the Fate")
install(TARGETS ${EXECUTABLE_TARGET_NAME} RUNTIME DESTINATION . BUNDLE DESTINATION .)
install(FILES ${CMAKE_SOURCE_DIR}/etf.sg DESTINATION data)
if(STEAM_ENABLED AND NOT APPLE)
    install(FILES "${STEAM_RUNTIME_FILES}" DESTINATION .)
endif()
if(WIN32)
    set(CPACK_GENERATOR "NSIS")
elseif(APPLE)
    if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
        set(CPACK_GENERATOR "ZIP")
    else()
        set(CPACK_GENERATOR "DragNDrop")
    endif()
else()
    set(CPACK_GENERATOR "TGZ")
endif()
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/packages")
include(CPack)
