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


#include "engine/renderer/rendermanager.hpp"

#include <iostream>
#include <sstream>
#include "engine/renderer/renderer.hpp"
#include "engine/renderer/camera.hpp"
#include "engine/renderer/renderablemesh.hpp"

using namespace std;

Renderer* RenderManager::ms_renderer = 0;

Renderer* RenderManager::create() {
    if (ms_renderer == 0) {
        ms_renderer = new Renderer;
        ms_renderer->initialize();
    }
    else
        cerr << "Warning: renderer already exists, cannot create" << endl;
    return ms_renderer;
}

void RenderManager::shutdown() {
    if (ms_renderer != 0) {
        ms_renderer->deinitialize();
        delete ms_renderer;
        ms_renderer = 0;
    }
    else
        cerr << "Warning: no existing renderer, cannot shutdown" << endl;
}
