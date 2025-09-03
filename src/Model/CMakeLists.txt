set(MODEL_SOURCES
    Disassembler.cpp
    I8086.cpp
    IOBus.cpp
    MemoryBus.cpp
)

add_library(Model STATIC ${MODEL_SOURCES})

target_link_libraries(Model PUBLIC imgui)

target_include_directories(Model PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)