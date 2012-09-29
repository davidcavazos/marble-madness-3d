set(KERNEL_LIBRARY_NAME kernel)
set(RENDER_LIBRARY_NAME renderer)

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

add_library(${KERNEL_LIBRARY_NAME} SHARED ${KERNEL_SRC_FILES})
add_library(${RENDER_LIBRARY_NAME} SHARED ${RENDER_SRC_FILES})

# Link libraries
find_package(Boost REQUIRED)
find_package(SDL REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${SDL_INCLUDE_DIR}
    ${SDL_INCLUDE_DIR}/SDL
    ${OPENGL_INCLUDE_DIR}
)

target_link_libraries(${KERNEL_LIBRARY_NAME} ${SDL_LIBRARY})
target_link_libraries(${RENDER_LIBRARY_NAME} ${OPENGL_LIBRARIES})
