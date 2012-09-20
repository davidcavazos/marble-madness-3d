/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  David Cavazos <davido262@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "engine/device/devicemanager.hpp"

#include <iostream>
#include <cassert>

#include "engine/device/devicesdlopengl.hpp"

using namespace std;

Device* DeviceManager::ms_system = 0;

Device* DeviceManager::createSystem(const device_t type) {
    if (ms_system == 0) {
        switch (type) {
        case DEVICE_SDL_OPENGL_LEGACY:
            ms_system = new DeviceSDLOpenGL;
            break;
        default:
            cerr << "Invalid device type, returning null device" << endl;
        }
        assert(ms_system != 0);
        ms_system->initialize();
    }
    else
        cerr << "Warning: device already exists, cannot create" << endl;
    return ms_system;
}

void DeviceManager::shutdownSystem() {
    if (ms_system != 0) {
        ms_system->shutdown();
        delete ms_system;
        ms_system = 0;
    }
    else
        cerr << "Warning: no existing device, cannot shutdown" << endl;
}
