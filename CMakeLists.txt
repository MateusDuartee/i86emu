cmake_minimum_required(VERSION 3.20)
include(FetchContent)
project(i86emu LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release)

set(FETCHCONTENT_BASE_DIR ${CMAKE_BINARY_DIR}/_deps)

set(SDL_SHARED ON CACHE BOOL "" FORCE)
set(SDL_STATIC OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-3.2.20
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)

FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.92.2b-docking
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)

FetchContent_MakeAvailable(imgui)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.12.0
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)

FetchContent_MakeAvailable(json)

SET(IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

find_package(OpenGL REQUIRED)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_link_libraries(imgui PUBLIC
    SDL3::SDL3
    OpenGL::GL
)

target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${sdl3_SOURCE_DIR}/include
)

set(PFD_URL "https://raw.githubusercontent.com/samhocevar/portable-file-dialogs/master/portable-file-dialogs.h")
set(PFD_DEST "${CMAKE_BINARY_DIR}/_deps/pfd/portable-file-dialogs.h")

file(DOWNLOAD ${PFD_URL} ${PFD_DEST}
    STATUS DOWNLOAD_STATUS
    LOG DOWNLOAD_LOG
)

list(GET DOWNLOAD_STATUS 0 DOWNLOAD_CODE)
if(NOT DOWNLOAD_CODE EQUAL 0)
    message(FATAL_ERROR "Failed to download portable-file-dialogs.h: ${DOWNLOAD_LOG}")
endif()

set(MEM_EDITOR_URL "https://raw.githubusercontent.com/ocornut/imgui_club/refs/heads/main/imgui_memory_editor/imgui_memory_editor.h")
set(MEM_EDITOR_DEST "${imgui_SOURCE_DIR}/imgui_memory_editor.h")

file(DOWNLOAD ${MEM_EDITOR_URL} ${MEM_EDITOR_DEST}
    STATUS DOWNLOAD_STATUS
    LOG DOWNLOAD_LOG
)
list(GET DOWNLOAD_STATUS 0 DOWNLOAD_CODE)
if(NOT DOWNLOAD_CODE EQUAL 0)
    message(FATAL_ERROR "Failed to download imgui_memory_editor.h: ${DOWNLOAD_LOG}")
endif()

add_subdirectory(src)