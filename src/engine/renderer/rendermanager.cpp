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
#include <engine/renderer/renderablemesh.hpp>

using namespace std;

Renderer* RenderManager::ms_renderer = 0;
Camera* RenderManager::ms_activeCamera = 0;
set<Camera*> RenderManager::ms_cameras = set<Camera*>();
set<RenderableMesh*> RenderManager::ms_meshes = set<RenderableMesh*>();

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

string RenderManager::listsToString() {
    stringstream ss;
    ss << "Renderer Cameras List:" << endl;
    set<Camera*>::const_iterator itCam;
    for (itCam = ms_cameras.begin(); itCam != ms_cameras.end(); ++itCam) {
        ss << "  " << (*itCam)->getDescription();
        if (*itCam == ms_activeCamera)
            ss << " *";
        ss << endl;
    }
    ss << endl;

    ss << "Renderer Meshes List:" << endl;
    set<RenderableMesh*>::const_iterator itMesh;
    for (itMesh = ms_meshes.begin(); itMesh != ms_meshes.end(); ++itMesh)
        ss << "  " << (*itMesh)->getDescription() << endl;

    return ss.str();
}
