set(EXE_NAME marble-madness-3d)

# Target
set(SRC_FILES
    main.cpp
)
set(LIBRARIES
    terminal
    device-manager
)

add_executable(${EXE_NAME} ${SRC_FILES})

# Link libraries
target_link_libraries(${EXE_NAME} ${LIBRARIES})

add_subdirectory(terminal)
add_subdirectory(device)
