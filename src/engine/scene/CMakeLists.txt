set(LIBRARY_NAME scene-manager)

# Target
set(SRC_FILES
    entity.cpp
    component.cpp
)

add_library(${LIBRARY_NAME} SHARED ${SRC_FILES})

# Link libraries
find_package(Boost REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
