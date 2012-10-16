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


#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

class Mesh {
public:
    const std::vector<float>& getVertices() const;
    const std::vector<float>& getNormals() const;
    const std::vector<unsigned char>& getIndices() const;

private:
    std::vector<float> m_vertices;
    std::vector<float> m_normals;
    std::vector<unsigned char> m_indices;
};



inline const std::vector<float>& Mesh::getVertices() const {
    return m_vertices;
}

inline const std::vector<float>& Mesh::getNormals() const {
    return m_normals;
}

inline const std::vector<unsigned int>& Mesh::getIndices() const {
    return m_indices;
}

#endif // MESH_HPP
