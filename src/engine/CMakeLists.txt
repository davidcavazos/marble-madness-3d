set(LIBRARY_NAME engine)

set(SRC_FILES
    tokentable.cpp
    commandobject.cpp
    command.cpp
    terminal.cpp

    device.cpp
    devicemanager.cpp
    inputmanager.cpp

    entity.cpp
    component.cpp
    scenemanager.cpp
)

add_library(${LIBRARY_NAME} SHARED ${SRC_FILES})

# Link libraries
find_package(Boost REQUIRED)
find_package(SDL REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} ${SDL_INCLUDE_DIR} ${SDL_INCLUDE_DIR}/SDL)
target_link_libraries(${LIBRARY_NAME} ${SDL_LIBRARY})
