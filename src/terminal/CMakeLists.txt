set(LIBRARY_NAME terminal)

set(SRC_FILES
    tokentable.cpp
    commandobject.cpp
    command.cpp
    terminal.cpp
)

add_library(${LIBRARY_NAME} SHARED ${SRC_FILES})

# Link libraries
find_package(Boost REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
