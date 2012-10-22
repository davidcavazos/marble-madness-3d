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


#ifndef RENDERABLEMESH_HPP
#define RENDERABLEMESH_HPP

#include "engine/kernel/component.hpp"

class Model;

class RenderableMesh: public Component {
public:
    RenderableMesh(Entity* const entity);
    ~RenderableMesh();

    const Model* getMeshDataPtr() const;
    const Model& getMeshData() const;

    void loadBox(const double lengthX, const double lengthY, const double lengthZ);
    void loadFromFile(const std::string& fileName);

private:
    Model* m_meshData;

    // Should not be called
    RenderableMesh(const RenderableMesh& rhs);
    RenderableMesh& operator=(const RenderableMesh& rhs);
};



inline const Model* RenderableMesh::getMeshDataPtr() const {
    return m_meshData;
}

inline const Model& RenderableMesh::getMeshData() const {
    return *getMeshDataPtr();
}



inline RenderableMesh::RenderableMesh(const RenderableMesh& rhs):
    Component(rhs.m_type, &rhs.m_entity),
    m_meshData(rhs.m_meshData)
{}

inline RenderableMesh& RenderableMesh::operator=(const RenderableMesh&) {
    return *this;
}

#endif // RENDERABLEMESH_HPP
