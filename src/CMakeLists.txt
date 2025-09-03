SET(SOURCES
    main.cpp
)

add_executable(i86emu 
    ${SOURCES}
)

target_link_libraries(i86emu PRIVATE
    Core
)

set(BIN_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_custom_command(TARGET i86emu POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE_DIR:i86emu>)

add_subdirectory(Core)
add_subdirectory(Interfaces)
add_subdirectory(Model)
add_subdirectory(View)
add_subdirectory(Controller)