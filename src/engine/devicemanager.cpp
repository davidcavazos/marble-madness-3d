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

using namespace std;

Device* DeviceManager::ms_manager = 0;

Device* DeviceManager::create() {
    if (ms_manager == 0) {
        ms_manager = new Device;
        ms_manager->initialize();
    }
    else
        cerr << "Warning: device already exists, cannot create" << endl;
    return ms_manager;
}

void DeviceManager::shutdown() {
    if (ms_manager != 0) {
        ms_manager->shutdown();
        delete ms_manager;
        ms_manager = 0;
    }
    else
        cerr << "Warning: no existing device, cannot shutdown" << endl;
}
