#include <iostream>

#include <iostream>
#include <cstdlib>
#include "device/devicemanager.hpp"

using namespace std;

int main(int argc, char** argv) {
    cout << argc << *argv << endl;

    Device* device = DeviceManager::createSystem(DEVICE_SDL_OPENGL_LEGACY, 640, 400);

    device->shutdown();

    return EXIT_SUCCESS;
}
