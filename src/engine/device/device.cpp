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


#include "engine/device/device.hpp"

const size_t DEFAULT_SCREEN_WIDTH = 640;
const size_t DEFAULT_SCREEN_HEIGHT = 480;
const size_t DEFAULT_SCREEN_DEPTH = 32;

InputManager Device::ms_inputManager = InputManager();

Device::Device() :
    m_width(DEFAULT_SCREEN_WIDTH),
    m_height(DEFAULT_SCREEN_HEIGHT),
    m_depth(DEFAULT_SCREEN_DEPTH)
{}
