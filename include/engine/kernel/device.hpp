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


#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <string>
#include <set>
#include "inputmanager.hpp"

class DeviceManager;
struct SDL_Surface;

class Device {
public:
    friend class DeviceManager;

    InputManager& getInputManager();
    double getDeltaTime() const;
    double getFps() const;

    void onFrameStart();
    void onFrameEnd();
    void swapBuffers();
    size_t videoMemKB();
    void setTitle(const std::string& title);
    void setFullscreen(const bool useFullscreen = true);
    void setResolution(const size_t width, const size_t height);
    size_t getWinWidth() const;
    size_t getWinHeight() const;
    void processEvents(bool& isRunning);

protected:
    size_t m_width;
    size_t m_height;
    size_t m_halfWidth;
    size_t m_halfHeight;
    size_t m_depth;
    std::set<size_t> m_keysPressed;
    std::set<size_t> m_mouseButtonsPressed;
    static InputManager ms_inputManager;
    static SDL_Surface* ms_screen;
    double m_startTime;
    double m_deltaTime;
    double m_fps;

    Device();

    void initialize();
    void deinitialize();
};



inline InputManager& Device::getInputManager() {
    return ms_inputManager;
}

inline double Device::getDeltaTime() const {
    return m_deltaTime;
}

inline double Device::getFps() const {
    return m_fps;
}

#endif // DEVICE_HPP
