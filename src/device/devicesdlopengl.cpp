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


#include "devicesdlopengl.hpp"

#include <iostream>
#include <cassert>
#include <SDL/SDL.h>
#include "inputsdl.hpp"

using namespace std;

const Uint32 SDL_INIT_FLAGS = SDL_INIT_VIDEO;// | SDL_INIT_JOYSTICK;
const Uint32 SDL_VIDEO_FLAGS = SDL_HWSURFACE | SDL_ANYFORMAT | SDL_OPENGL;

SDL_Surface* DeviceSDLOpenGL::m_screen = 0;

size_t DeviceSDLOpenGL::videoMemKB() {
    const SDL_VideoInfo* info = SDL_GetVideoInfo();
    return info->video_mem;
}

void DeviceSDLOpenGL::setTitle(const std::string& title) {
    SDL_WM_SetCaption(title.c_str(), title.c_str());
}

void DeviceSDLOpenGL::setFullscreen(const bool useFullscreen) {
    m_screen = SDL_GetVideoSurface();
    Uint32 flags = m_screen->flags;
    Uint32 fullscreenBit = useFullscreen? SDL_FULLSCREEN : 0;
    m_screen = SDL_SetVideoMode(0, 0, 0, flags | fullscreenBit);
    if (m_screen == 0)
        m_screen = SDL_SetVideoMode(0, 0, 0, flags);
    if (m_screen == 0)
        exit(1);
}

void DeviceSDLOpenGL::setResolution(const size_t width, const size_t height) {
    Uint32 flags = SDL_GetVideoSurface()->flags;
    m_screen = SDL_SetVideoMode(width, height, 0, flags);
    m_width = static_cast<size_t>(m_screen->w);
    m_height = static_cast<size_t>(m_screen->h);
}

void DeviceSDLOpenGL::processEvents(bool& isRunning) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                isRunning = false;
            }
            break;
        }
    }
}

void DeviceSDLOpenGL::initialize() {
    cout << "Creating SDL-OpenGL device" << endl;
    int init = SDL_Init(SDL_INIT_FLAGS);
    assert(init == 0);

    const SDL_VideoInfo* info = SDL_GetVideoInfo();
    m_width = info->current_w;
    m_height = info->current_h;
    m_depth = info->vfmt->BitsPerPixel;

    m_screen = SDL_SetVideoMode(m_width, m_height, m_depth, SDL_VIDEO_FLAGS);
    assert(m_screen != 0);
}

void DeviceSDLOpenGL::shutdown() {
    cout << "SDL-OpenGL device quit" << endl;
    SDL_Quit();
}
