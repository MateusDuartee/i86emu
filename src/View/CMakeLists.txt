set(VIEW_SOURCES
    DisassemblerWindow.cpp
    StateWindow.cpp
    MemoryEditorWindow.cpp
)

add_library(View STATIC ${VIEW_SOURCES})

target_link_libraries(View PUBLIC Controller)

target_include_directories(View PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${imgui_SOURCE_DIR}
)