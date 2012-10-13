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


#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <boost/unordered_map.hpp>

class Resources;
class MeshData;

class ResourceManager {
public:
    friend class Resources;

    static Resources* getResourcesPtr();
    static Resources& getResources();

    static Resources* create();
    static void shutdown();
    static std::string listsToString();

private:
    typedef boost::unordered_map<std::string, MeshData*> mesh_data_map_t;

    static Resources* ms_resources;
    static mesh_data_map_t ms_meshDataMap;
};



inline Resources* ResourceManager::getResourcesPtr() {
    return ms_resources;
}

inline Resources& ResourceManager::getResources() {
    return *getResourcesPtr();
}

#endif // RESOURCEMANAGER_HPP
