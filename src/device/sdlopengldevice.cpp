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


#include "device/sdlopengldevice.hpp"

#include <iostream>
#include <SDL/SDL.h>

using namespace std;

bool SdlOpenGlDevice::init(const int width, const int height) {
    cout << "Creating SDL device" << endl;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return false;
    SDL_Surface* display = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE | SDL_OPENGL);
    if (display == 0)
        return false;
    return true;
}

void SdlOpenGlDevice::quit() {
    cout << "SDL device quit" << endl;
    SDL_Quit();
}
