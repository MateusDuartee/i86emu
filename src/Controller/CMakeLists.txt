set(CONTROLLER_SOURCES
    ColorThemeController.cpp
    RAMController.cpp
)

add_library(Controller STATIC ${CONTROLLER_SOURCES})

target_link_libraries(Controller PUBLIC 
    Model
    nlohmann_json::nlohmann_json
)

target_include_directories(Controller PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${json_SOURCE_DIR}/include
)