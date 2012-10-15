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


#include "engine/resources/resources.hpp"

#include "engine/resources/resourcemanager.hpp"
#include "engine/resources/meshdata.hpp"
#include <engine/resources/generalmeshloader.hpp>

using namespace std;

MeshData* Resources::loadCube(const string& identifier, const double lengthX, const double lengthY, const double lengthZ) {
    MeshData* meshData;
    meshData = findMeshData(identifier);
    if (meshData != 0)
        return meshData;

    float x = static_cast<float>(lengthX / 2.0);
    float y = static_cast<float>(lengthY / 2.0);
    float z = static_cast<float>(lengthZ / 2.0);
    float vertices[] = {
         x,  y,  z,  -x,  y,  z,  -x, -y,  z,   x, -y,  z, // v0,v1,v2,v3 (front)
         x,  y,  z,   x, -y,  z,   x, -y, -z,   x,  y, -z, // v0,v3,v4,v5 (right)
         x,  y,  z,   x,  y, -z,  -x,  y, -z,  -x,  y,  z, // v0,v5,v6,v1 (top)
        -x,  y,  z,  -x,  y, -z,  -x, -y, -z,  -x, -y,  z, // v1,v6,v7,v2 (left)
        -x, -y, -z,   x, -y, -z,   x, -y,  z,  -x, -y,  z, // v7,v4,v3,v2 (bottom)
         x, -y, -z,  -x, -y, -z,  -x,  y, -z,   x,  y, -z  // v4,v7,v6,v5 (back)
    };
    float normals[] = {
         0,  0,  1,   0,  0,  1,   0,  0,  1,   0,  0,  1, // v0,v1,v2,v3 (front)
         1,  0,  0,   1,  0,  0,   1,  0,  0,   1,  0,  0, // v0,v3,v4,v5 (right)
         0,  1,  0,   0,  1,  0,   0,  1,  0,   0,  1,  0, // v0,v5,v6,v1 (top)
        -1,  0,  0,  -1,  0,  0,  -1,  0,  0,  -1,  0,  0, // v1,v6,v7,v2 (left)
         0, -1,  0,   0, -1,  0,   0, -1,  0,   0, -1,  0, // v7,v4,v3,v2 (bottom)
         0,  0, -1,   0,  0, -1,   0,  0, -1,   0,  0, -1  // v4,v7,v6,v5 (back)
    };
    unsigned int indices[] = {
         0,  1,  2,   2,  3,  0,  // front
         4,  5,  6,   6,  7,  4,  // right
         8,  9, 10,  10, 11,  8,  // top
        12, 13, 14,  14, 15, 12,  // left
        16, 17, 18,  18, 19, 16,  // bottom
        20, 21, 22,  22, 23, 20   // back
    };

    meshData = new MeshData(identifier);
    meshData->setTotalSubmeshes(1);
    meshData->setVertices(0, vertices, 72);
    meshData->setNormals(0, normals, 72);
    meshData->setIndices(0, indices, 36);
    registerMeshData(meshData);
    return meshData;
}

MeshData* Resources::loadMeshFromFile(const std::string& fileName) {
    MeshData* meshData;
    meshData = findMeshData(fileName);
    if (meshData != 0)
        return meshData;

    meshData = new MeshData(fileName);
    GeneralMeshLoader::load(fileName, meshData->getSubmeshes());
    registerMeshData(meshData);
    return meshData;
}

Resources::Resources() {
}

void Resources::initialize() {
}

void Resources::deinitialize() {
}



void Resources::registerMeshData(MeshData* meshData) {
    ResourceManager::ms_meshDataMap.insert(pair<string, MeshData*>(meshData->getIdentifier(), meshData));
}

MeshData* Resources::findMeshData(const std::string& identifier) {
    ResourceManager::mesh_data_map_t::const_iterator it;
    it = ResourceManager::ms_meshDataMap.find(identifier);
    if (it != ResourceManager::ms_meshDataMap.end())
        return it->second;
    return 0;
}
