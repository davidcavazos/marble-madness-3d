/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  David Cavazos <email>

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


#include "device/devicefactory.hpp"

#include <iostream>
#include "device/sdlopengldevice.hpp"

using namespace std;

DeviceSystem* DeviceFactory::newProduct(const device_t deviceType) {
    switch (deviceType) {
    case DEVICE_SDL_OPENGL_LEGACY:
        return new SdlOpenGlDevice;
    default:
        cout << "Invalid device type, returning null device" << endl;
    }
    return 0;
}
