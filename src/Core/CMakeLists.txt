set(CORE_SOURCES
    Application.cpp
    EmulatorApp.cpp
)

add_library(Core STATIC ${CORE_SOURCES})

target_link_libraries(Core PUBLIC 
    View
)

target_include_directories(Core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${CMAKE_BINARY_DIR}/_deps/pfd
)