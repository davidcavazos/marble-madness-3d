set(LIBRARY_NAME device-manager)

# Target
set(SRC_FILES
    devicesystem.cpp
    devicefactory.cpp
    devicemanager.cpp
    sdlopengldevice.cpp
)

add_library(${LIBRARY_NAME} SHARED ${SRC_FILES})

# Link libraries
find_package(Boost REQUIRED)
find_package(SDL REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} ${SDL_INCLUDE_DIR})
target_link_libraries(${LIBRARY_NAME} ${SDL_LIBRARY})
