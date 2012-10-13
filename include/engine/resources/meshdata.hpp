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


#ifndef MESHDATA_HPP
#define MESHDATA_HPP

#include <vector>
#include <string>

class MeshData {
public:
    MeshData(const std::string& identifier);

    const std::string& getIdentifier() const;
    const std::vector<float>& getVertices() const;
    const float* getVerticesPtr() const;
    const std::vector<float>& getNormals() const;
    const float* getNormalsPtr() const;
    const std::vector<unsigned char>& getIndices() const;
    const unsigned char* getIndicesPtr() const;

    void setIdentifier(const std::string& identifier);
    void setVertices(const std::vector<float>& vertices);
    void setVertices(const float* vertices, const size_t size);
    void setNormals(const std::vector<float>& normals);
    void setNormals(const float* normals, const size_t size);
    void setIndices(const std::vector<unsigned char>& indices);
    void setIndices(const unsigned char* indices, const size_t size);

private:
    std::string m_identifier;
    std::vector<float> m_vertices;
    std::vector<float> m_normals;
    std::vector<unsigned char> m_indices;
};



inline const std::string& MeshData::getIdentifier() const {
    return m_identifier;
}

inline const std::vector<float>& MeshData::getVertices() const {
    return m_vertices;
}

inline const float* MeshData::getVerticesPtr() const {
    return &m_vertices[0];
}

inline const std::vector<float>& MeshData::getNormals() const {
    return m_normals;
}

inline const float* MeshData::getNormalsPtr() const {
    return &m_normals[0];
}

inline const std::vector<unsigned char>& MeshData::getIndices() const {
    return m_indices;
}

inline const unsigned char* MeshData::getIndicesPtr() const {
    return &m_indices[0];
}



inline void MeshData::setIdentifier(const std::string& identifier) {
    m_identifier = identifier;
}

inline void MeshData::setVertices(const std::vector<float>& vertices) {
    m_vertices = vertices;
}

inline void MeshData::setVertices(const float* vertices, const size_t size) {
    m_vertices.resize(size);
    for (size_t i = 0; i < m_vertices.size(); ++i)
        m_vertices[i] = vertices[i];
}

inline void MeshData::setNormals(const std::vector<float>& normals) {
    m_normals = normals;
}

inline void MeshData::setNormals(const float* normals, const size_t size) {
    m_normals.resize(size);
    for (size_t i = 0; i < m_normals.size(); ++i)
        m_normals[i] = normals[i];
}

inline void MeshData::setIndices(const std::vector<unsigned char>& indices) {
    m_indices = indices;
}

inline void MeshData::setIndices(const unsigned char* indices, const size_t size) {
    m_indices.resize(size);
    for (size_t i = 0; i < m_indices.size(); ++i)
        m_indices[i] = indices[i];
}

#endif // MESHDATA_HPP
