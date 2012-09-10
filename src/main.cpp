#include <iostream>

#include <iostream>
#include <cstdlib>
#include "device/devicemanager.hpp"

#include <SDL/SDL.h>

using namespace std;

int main(int, char**) {
    Device* device = DeviceManager::createSystem(DEVICE_SDL_OPENGL_LEGACY);
    device->setTitle("Marble Madness 3D");
    device->setResolution(800, 500);

    bool isRunning = true;
    while (isRunning) {
        device->processEvents(isRunning);
    }

    DeviceManager::shutdownSystem();

    return EXIT_SUCCESS;
}
