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
#include "engine/resources/texture.hpp"
#include "engine/resources/model.hpp"
#include "engine/resources/modelloader.hpp"

using namespace std;

Model* Resources::generateBox(const string& identifier, const double lengthX, const double lengthY, const double lengthZ) {
    Model* model;
    model = findModel(identifier);
    if (model != 0)
        return model;

    float x = static_cast<float>(lengthX * 0.5);
    float y = static_cast<float>(lengthY * 0.5);
    float z = static_cast<float>(lengthZ * 0.5);
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

    model = new Model(identifier);
    model->setTotalMeshes(1);
    model->mesh(0).setVertices(vertices, 72);
    model->mesh(0).setNormals(normals, 72);
    model->mesh(0).setIndices(indices, 36);
    registerModel(model);
    return model;
}

Model* Resources::generateModelFromFile(const std::string& fileName) {
    Model* model;
    model = findModel(fileName);
    if (model != 0)
        return model;

    model = new Model(fileName);
    ModelLoader::load(fileName, *model);
    registerModel(model);
    return model;
}

Resources::Resources():
    m_modelsMap(),
    m_texturesMap()
{}

void Resources::initialize() {
}

void Resources::deinitialize() {
    models_map_t::const_iterator itMesh;
    for (itMesh = m_modelsMap.begin(); itMesh != m_modelsMap.end(); ++itMesh)
        delete itMesh->second;
}

string Resources::listsToString() {
    stringstream ss;
    ss << "Mesh Data List:" << endl;
    models_map_t::const_iterator itMesh;
    for (itMesh = m_modelsMap.begin(); itMesh != m_modelsMap.end(); ++itMesh)
        ss << "  " << itMesh->second->getIdentifier() << endl;
    return ss.str();
}



void Resources::registerModel(Model* model) {
    ResourceManager::getResources().m_modelsMap.insert(pair<string, Model*>(model->getIdentifier(), model));
}

void Resources::registerTexture(Texture* texture) {
    ResourceManager::getResources().m_texturesMap.insert(pair<string, Texture*>(texture->getIdentifier(), texture));
}

Model* Resources::findModel(const std::string& identifier) {
    models_map_t::const_iterator it;
    it = ResourceManager::getResources().m_modelsMap.find(identifier);
    if (it != ResourceManager::getResources().m_modelsMap.end())
        return it->second;
    return 0;
}

Texture* Resources::findTexture(const std::string& identifier) {
    textures_map_t::const_iterator it;
    it = ResourceManager::getResources().m_texturesMap.find(identifier);
    if (it != ResourceManager::getResources().m_texturesMap.end())
        return it->second;
    return 0;
}
