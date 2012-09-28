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


#include "engine/device.hpp"

#include <iostream>
#include <cassert>
#include <SDL.h>

using namespace std;

const size_t DEFAULT_SCREEN_WIDTH = 640;
const size_t DEFAULT_SCREEN_HEIGHT = 480;
const size_t DEFAULT_SCREEN_DEPTH = 32;

const Uint32 SDL_INIT_FLAGS = SDL_INIT_VIDEO;// | SDL_INIT_JOYSTICK;
const Uint32 SDL_VIDEO_FLAGS = SDL_HWSURFACE | SDL_ANYFORMAT | SDL_OPENGL;

InputManager Device::ms_inputManager = InputManager();
SDL_Surface* Device::m_screen = 0;

Device::Device() :
    m_width(DEFAULT_SCREEN_WIDTH),
    m_height(DEFAULT_SCREEN_HEIGHT),
    m_depth(DEFAULT_SCREEN_DEPTH)
{}

size_t Device::videoMemKB() {
    const SDL_VideoInfo* info = SDL_GetVideoInfo();
    return info->video_mem;
}

void Device::setTitle(const std::string& title) {
    SDL_WM_SetCaption(title.c_str(), title.c_str());
}

void Device::setFullscreen(const bool useFullscreen) {
    m_screen = SDL_GetVideoSurface();
    Uint32 flags = m_screen->flags;
    Uint32 fullscreenBit = useFullscreen? SDL_FULLSCREEN : 0;
    m_screen = SDL_SetVideoMode(0, 0, 0, flags | fullscreenBit);
    if (m_screen == 0)
        m_screen = SDL_SetVideoMode(0, 0, 0, flags);
    if (m_screen == 0)
        exit(1);
}

void Device::setResolution(const size_t width, const size_t height) {
    Uint32 flags = SDL_GetVideoSurface()->flags;
    m_screen = SDL_SetVideoMode(width, height, 0, flags);
    m_width = static_cast<size_t>(m_screen->w);
    m_height = static_cast<size_t>(m_screen->h);
}

void Device::processEvents(bool& isRunning) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                ms_inputManager.onKeyDown(event.key.keysym.sym);
                break;
            case SDL_KEYUP:
                ms_inputManager.onKeyUp(event.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                ms_inputManager.onMouseButtonDown(event.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                ms_inputManager.onMouseButtonUp(event.button.button);
                break;
            case SDL_MOUSEMOTION: {
                mouse_motion_t motion;
                motion.x = event.motion.x;
                motion.y = event.motion.y;
                motion.xrel = event.motion.xrel;
                motion.yrel = event.motion.yrel;
                ms_inputManager.onMouseMotion(motion);
                break; }
        }
    }
}

void Device::getPointerInfo(int& x, int& y) {
    SDL_GetMouseState(&x, &y);
}

void Device::initialize() {
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

void Device::shutdown() {
    cout << "SDL-OpenGL device quit" << endl;
    SDL_Quit();
}
