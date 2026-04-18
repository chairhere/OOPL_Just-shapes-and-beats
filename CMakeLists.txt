cmake_minimum_required(VERSION 3.16)
project(JUST_SHAPES_AND_BEATS)

set(CMAKE_CXX_STANDARD 17)

cmake_policy(SET CMP0135 NEW)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
    ptsd

    URL         https://github.com/ntut-open-source-club/practical-tools-for-simple-design/archive/refs/tags/v0.2.zip

    # Or alternatively if you want to use a specific git commit instead of a release:
    # GIT_REPOSITORY  https://github.com/ntut-open-source-club/practical-tools-for-simple-design.git
    # GIT_TAG         <git_tag>

    SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/PTSD
)

FetchContent_MakeAvailable(ptsd)

include(files.cmake)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
list(TRANSFORM SRC_FILES PREPEND ${SRC_DIR}/)
list(TRANSFORM INCLUDE_FILES PREPEND ${INCLUDE_DIR}/)

# file(GLOB_RECURSE SRC_FILES src/*)
# file(GLOB_RECURSE HEADER_FILES include/*)
# ================= [ SoLoud 音訊引擎設定開始 ] =================

# 1. 告訴編譯器 SoLoud 的標頭檔 (.h) 在哪裡
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/soloud/include)

# 2. 收集 SoLoud 必須編譯的原始碼 (精準抓取，不搞花招)
file(GLOB SOLOUD_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/soloud/src/core/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/soloud/src/audiosource/wav/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/soloud/src/backend/miniaudio/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/soloud/src/backend/miniaudio/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/soloud/src/audiosource/wav/stb_vorbis.c"
)

# 3. 定義巨集：告訴 SoLoud 我們要用 Miniaudio 作為底層
add_definitions(-DWITH_MINIAUDIO)

# ================= [ SoLoud 音訊引擎設定結束 ] =================

add_executable(${PROJECT_NAME}
    ${SRC_DIR}/main.cpp
    ${SRC_FILES}
    ${HEADER_FILES}
        "src/TimeLine.cpp"
        "include/TimeLine.hpp"
        "include/Player.hpp"
        "src/Player.cpp"
        "include/SpawnEvent.hpp"
        "include/SongsBPM.hpp"
        "src/Button.cpp"
        "include/Button.hpp"
        "include/Obstacle.hpp"
        src/Obstacle.cpp
        include/ScreenState.hpp
        include/Screen.hpp
        src/MainMenuScreen.cpp
        include/MainMenuScreen.hpp
        src/LevelSpawner.cpp
        include/LevelSpawner.hpp
        include/CustomColorShape.hpp
        src/CustomColorShape.cpp
        include/FadeLayer.hpp
        src/FadeLayer.cpp
        include/PlaygroundScreen.hpp
        src/PlaygroundScreen.cpp
        include/Levels.hpp
        src/SongListScreen.cpp
        include/SongListScreen.hpp
        include/SongList.hpp
        include/SongData.hpp
        include/TestShape.hpp
        src/MusicPlayerManager.cpp
        include/MusicPlayerManager.hpp
        src/Label.cpp
        include/Label.hpp
        include/Label.hpp
        src/TextObject.cpp
        include/TextObject.hpp
        src/ImageObject.cpp
        include/ImageObject.hpp
        src/EventObject.cpp
        include/EventObject.hpp
        src/SongListItem.cpp
        include/SongListItem.hpp
        include/BatchedColorShape.hpp
        src/BatchedColorShape.cpp
        include/BulletType.hpp
    ${SOLOUD_SOURCES}
        include/BatchedCircleShape.hpp
        src/BatchedCircleShape.cpp
        src/DiscreteSlider.cpp
        include/DiscreteSlider.hpp
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Resources")
else()
    message(FATAL_ERROR "relative RESOURCE_DIR is WIP, Please use `-DCMAKE_BUILD_TYPE=Debug` build for now.")
    # target_compile_definitions(${PROJECT_NAME} PRIVATE RESOURCE_DIR="${CMAKE_INSTALL_PREFIX}/Resources")
endif()

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${DEPENDENCY_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/PTSD/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}
    SDL2::SDL2main
    PTSD
)
