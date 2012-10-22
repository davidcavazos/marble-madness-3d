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

#include "material.hpp"

class Mesh {
public:
    std::vector<float> m_vertices;
    std::vector<float> m_normals;
    std::vector<unsigned int> m_indices;
    std::vector<Material> m_materials;

    Mesh();

    const std::vector<float>& getVertices() const;
    const float* getVerticesPtr() const;
    size_t getTotalVertices() const;
    const std::vector<float>& getNormals() const;
    const float* getNormalsPtr() const;
    size_t getTotalNormals() const;
    const std::vector<unsigned int>& getIndices() const;
    const unsigned int* getIndicesPtr() const;
    size_t getTotalIndices() const;
    const Material& getMaterial(const size_t index) const;
    size_t getTotalMaterials() const;

    Material& material(const size_t index);

    void setVertices(const std::vector<float>& vertices);
    void setVertices(const float* vertices, const size_t size);
    void setNormals(const std::vector<float>& normals);
    void setNormals(const float* normals, const size_t size);
    void setIndices(const std::vector<unsigned int>& indices);
    void setIndices(const unsigned int* indices, const size_t size);
};



inline const std::vector< float >& Mesh::getVertices() const {
    return m_vertices;
}

inline const float* Mesh::getVerticesPtr() const {
    return &m_vertices[0];
}

inline size_t Mesh::getTotalVertices() const {
    return m_vertices.size();
}

inline const std::vector< float >& Mesh::getNormals() const {
    return m_normals;
}

inline const float* Mesh::getNormalsPtr() const {
    return &m_normals[0];
}

inline size_t Mesh::getTotalNormals() const {
    return m_normals.size();
}

inline const std::vector< unsigned int >& Mesh::getIndices() const {
    return m_indices;
}

inline const unsigned int* Mesh::getIndicesPtr() const {
    return &m_indices[0];
}

inline size_t Mesh::getTotalIndices() const {
    return m_indices.size();
}

inline const Material& Mesh::getMaterial(const size_t index) const {
    return m_materials[index];
}

inline size_t Mesh::getTotalMaterials() const {
    return m_materials.size();
}

inline Material& Mesh::material(const size_t index) {
    return m_materials[index];
}



inline void Mesh::setVertices(const std::vector< float >& vertices) {
    m_vertices = vertices;
}

inline void Mesh::setVertices(const float* vertices, const size_t size) {
    m_vertices.resize(size);
    for (size_t i = 0; i < size; ++i)
        m_vertices[i] = vertices[i];
}

inline void Mesh::setNormals(const std::vector< float >& normals) {
    m_normals = normals;
}

inline void Mesh::setNormals(const float* normals, const size_t size) {
    m_normals.resize(size);
    for (size_t i = 0; i < size; ++i)
        m_normals[i] = normals[i];
}

inline void Mesh::setIndices(const std::vector< unsigned int >& indices) {
    m_indices = indices;
}

inline void Mesh::setIndices(const unsigned int* indices, const size_t size) {
    m_indices.resize(size);
    for (size_t i = 0; i < size; ++i)
        m_indices[i] = indices[i];
}

#endif // MESH_HPP
