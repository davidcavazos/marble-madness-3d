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


#include "engine/kernel/devicemanager.hpp"

#include <iostream>
#include "engine/kernel/device.hpp"

using namespace std;

Device* DeviceManager::ms_device = 0;

Device* DeviceManager::create() {
    if (ms_device == 0) {
        ms_device = new Device;
        ms_device->initialize();
    }
    else
        cerr << "Warning: device already exists, cannot create" << endl;
    return ms_device;
}

void DeviceManager::shutdown() {
    if (ms_device != 0) {
        ms_device->deinitialize();
        delete ms_device;
        ms_device = 0;
    }
    else
        cerr << "Warning: no existing device, cannot shutdown" << endl;
}

double DeviceManager::getDeltaTime() {
    return ms_device->m_deltaTime;
}

double DeviceManager::getFps() {
    return ms_device->m_fps;
}
