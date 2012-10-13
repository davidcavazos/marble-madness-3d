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


#include "engine/renderer/renderablemesh.hpp"

#include <sstream>
#include "engine/renderer/rendermanager.hpp"
#include "engine/resources/meshdata.hpp"
#include "engine/resources/resourcemanager.hpp"
#include "engine/resources/resources.hpp"

using namespace std;

const string CUBE_DESCRIPTION = "$cube";

RenderableMesh::RenderableMesh(Entity* const entity):
    Component(COMPONENT_RENDERABLE_MESH, entity),
    m_meshData(0)
{
    RenderManager::ms_meshes.insert(this);
}

RenderableMesh::~RenderableMesh() {
    RenderManager::ms_meshes.erase(this);
}



void RenderableMesh::loadCube(const double lengthX, const double lengthY, const double lengthZ) {
    stringstream ss;
    ss << CUBE_DESCRIPTION << "_" << lengthX << "_" << lengthY << "_" << lengthZ;
    m_description = ss.str();
    m_meshData = ResourceManager::getResources().loadCube(m_description, lengthX, lengthY, lengthZ);
}

void RenderableMesh::loadFromFile(const string& fileName) {
    m_description = fileName;
    m_meshData = ResourceManager::getResources().loadFromFile(fileName);
}
