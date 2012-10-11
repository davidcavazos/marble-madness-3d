set(KERNEL_LIBRARY_NAME kernel)
set(RENDER_LIBRARY_NAME renderer)
set(RESOURCES_LIBRARY_NAME resources)

set(KERNEL_SRC_FILES
    kernel/tokentable.cpp
    kernel/commandobject.cpp
    kernel/command.cpp
    kernel/terminal.cpp

    kernel/device.cpp
    kernel/inputmanager.cpp
    kernel/devicemanager.cpp

    kernel/entity.cpp
    kernel/component.cpp
    kernel/scenemanager.cpp
)

set(RENDER_SRC_FILES
    renderer/camera.cpp
    renderer/renderablemesh.cpp
    renderer/renderer.cpp
    renderer/rendermanager.cpp
)

set(RESOURCES_SRC_FILES
    resources/resources.cpp
    resources/resourcemanager.cpp
)

add_library(${KERNEL_LIBRARY_NAME} SHARED ${KERNEL_SRC_FILES})
add_library(${RENDER_LIBRARY_NAME} SHARED ${RENDER_SRC_FILES})
add_library(${RESOURCES_LIBRARY_NAME} SHARED ${RESOURCES_SRC_FILES})

# Link libraries
find_package(Boost REQUIRED)
find_package(SDL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Bullet REQUIRED)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${SDL_INCLUDE_DIR}
    ${SDL_INCLUDE_DIR}/SDL
    ${OPENGL_INCLUDE_DIR}
    ${BULLET_INCLUDE_DIRS}
)

target_link_libraries(${KERNEL_LIBRARY_NAME} ${SDL_LIBRARY})
target_link_libraries(${RENDER_LIBRARY_NAME} ${OPENGL_LIBRARIES})
