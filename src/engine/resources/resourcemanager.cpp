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


#include "engine/resources/resourcemanager.hpp"

#include <iostream>
#include <sstream>
#include "engine/resources/resources.hpp"
#include "engine/resources/meshdata.hpp"

using namespace std;

Resources* ResourceManager::ms_resources = 0;
ResourceManager::mesh_data_map_t ResourceManager::ms_meshDataMap = mesh_data_map_t();


Resources* ResourceManager::create() {
    if (ms_resources == 0) {
        ms_resources = new Resources;
        ms_resources->initialize();
    }
    else
        cerr << "Warning: resources instance already exists, cannot create" << endl;
    return ms_resources;
}

void ResourceManager::shutdown() {
    if (ms_resources != 0) {
        ms_resources->deinitialize();
        delete ms_resources;
        ms_resources = 0;
    }
    else
        cerr << "Warning: no resources instance, cannot shutdown" << endl;

    mesh_data_map_t::const_iterator itMesh;
    for (itMesh = ms_meshDataMap.begin(); itMesh != ms_meshDataMap.end(); ++itMesh)
        delete itMesh->second;
}

string ResourceManager::listsToString() {
    stringstream ss;
    ss << "Mesh Data List:" << endl;
    mesh_data_map_t::const_iterator itMesh;
    for (itMesh = ms_meshDataMap.begin(); itMesh != ms_meshDataMap.end(); ++itMesh)
        ss << "  " << itMesh->second->getIdentifier() << endl;
    return ss.str();
}
