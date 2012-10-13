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


#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <string>

class ResourceManager;
class MeshData;

class Resources {
public:
    friend class ResourceManager;

    MeshData* loadCube(const std::string& identifier, const double lengthX, const double lengthY, const double lengthZ);
    MeshData* loadFromFile(const std::string& fileName);

private:
    Resources();

    void initialize();
    void deinitialize();

    void registerMeshData(MeshData* meshData);
    MeshData* findMeshData(const std::string& identifier);
};

#endif // RESOURCES_HPP
