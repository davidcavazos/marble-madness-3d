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

class Submesh {
public:
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<unsigned int> indices;
    std::vector<std::vector<float> > uvCoords;

    Submesh();
};

class MeshData {
public:
    MeshData(const std::string& identifier);

    const std::string& getIdentifier() const;
    size_t getTotalSubmeshes() const;
    const std::vector<Submesh>& getSubmeshes() const;
    std::vector<Submesh>& getSubmeshes();
    const std::vector<float>& getVertices(const size_t submesh) const;
    const float* getVerticesPtr(const size_t submesh) const;
    const std::vector<float>& getNormals(const size_t submesh) const;
    const float* getNormalsPtr(const size_t submesh) const;
    const std::vector<unsigned int>& getIndices(const size_t submesh) const;
    const unsigned int* getIndicesPtr(const size_t submesh) const;
    const std::vector<float>& getUvCoords(const size_t submesh, const size_t uvMap) const;
    const float* getUvCoordsPtr(const size_t submesh, const size_t uvMap) const;

    void setIdentifier(const std::string& identifier);
    void setTotalSubmeshes(const size_t submeshes);
    void setVertices(const size_t submesh, const std::vector<float>& vertices);
    void setVertices(const size_t submesh, const float* vertices, const size_t size);
    void setNormals(const size_t submesh, const std::vector<float>& normals);
    void setNormals(const size_t submesh, const float* normals, const size_t size);
    void setIndices(const size_t submesh, const std::vector<unsigned int>& indices);
    void setIndices(const size_t submesh, const unsigned int* indices, const size_t size);
    void setUvCoords(const size_t submesh, const size_t uvMap, const std::vector<float>& uvCoords);
    void setUvCoords(const size_t submesh, const size_t uvMap, const float* uvCoords, const size_t size);

private:
    std::string m_identifier;
    std::vector<Submesh> m_submeshes;
};



inline const std::string& MeshData::getIdentifier() const {
    return m_identifier;
}

inline size_t MeshData::getTotalSubmeshes() const {
    return m_submeshes.size();
}

inline const std::vector< Submesh >& MeshData::getSubmeshes() const {
    return m_submeshes;
}

inline std::vector< Submesh >& MeshData::getSubmeshes() {
    return m_submeshes;
}

inline const std::vector<float>& MeshData::getVertices(const size_t submesh) const {
    return m_submeshes[submesh].vertices;
}

inline const float* MeshData::getVerticesPtr(const size_t submesh) const {
    return &m_submeshes[submesh].vertices[0];
}

inline const std::vector<float>& MeshData::getNormals(const size_t submesh) const {
    return m_submeshes[submesh].normals;
}

inline const float* MeshData::getNormalsPtr(const size_t submesh) const {
    return &m_submeshes[submesh].normals[0];
}

inline const std::vector<unsigned int>& MeshData::getIndices(const size_t submesh) const {
    return m_submeshes[submesh].indices;
}

inline const unsigned int* MeshData::getIndicesPtr(const size_t submesh) const {
    return &m_submeshes[submesh].indices[0];
}

inline const std::vector<float>& MeshData::getUvCoords(const size_t submesh, const size_t uvMap) const {
    return m_submeshes[submesh].uvCoords[uvMap];
}

inline const float* MeshData::getUvCoordsPtr(const size_t submesh, const size_t uvMap) const {
    return &m_submeshes[submesh].uvCoords[uvMap][0];
}



inline void MeshData::setIdentifier(const std::string& identifier) {
    m_identifier = identifier;
}

inline void MeshData::setTotalSubmeshes(const size_t submeshes) {
    m_submeshes.resize(submeshes);
}

inline void MeshData::setVertices(const size_t submesh, const std::vector<float>& vertices) {
    m_submeshes[submesh].vertices = vertices;
}

inline void MeshData::setVertices(const size_t submesh, const float* vertices, const size_t size) {
    m_submeshes[submesh].vertices.resize(size);
    for (size_t i = 0; i < m_submeshes[submesh].vertices.size(); ++i)
        m_submeshes[submesh].vertices[i] = vertices[i];
}

inline void MeshData::setNormals(const size_t submesh, const std::vector<float>& normals) {
    m_submeshes[submesh].normals = normals;
}

inline void MeshData::setNormals(const size_t submesh, const float* normals, const size_t size) {
    m_submeshes[submesh].normals.resize(size);
    for (size_t i = 0; i < m_submeshes[submesh].normals.size(); ++i)
        m_submeshes[submesh].normals[i] = normals[i];
}

inline void MeshData::setIndices(const size_t submesh, const std::vector<unsigned int>& indices) {
    m_submeshes[submesh].indices = indices;
}

inline void MeshData::setIndices(const size_t submesh, const unsigned int* indices, const size_t size) {
    m_submeshes[submesh].indices.resize(size);
    for (size_t i = 0; i < m_submeshes[submesh].indices.size(); ++i)
        m_submeshes[submesh].indices[i] = indices[i];
}

inline void MeshData::setUvCoords(const size_t submesh, const size_t uvMap, const std::vector<float>& uvCoords) {
    m_submeshes[submesh].uvCoords[uvMap] = uvCoords;
}

inline void MeshData::setUvCoords(const size_t submesh, const size_t uvMap, const float* uvCoords, const size_t size) {
    m_submeshes[submesh].uvCoords[uvMap].resize(size);
    for (size_t i = 0; i < m_submeshes[submesh].uvCoords[uvMap].size(); ++i)
        m_submeshes[submesh].uvCoords[uvMap][i] = uvCoords[i];
}

#endif // MESHDATA_HPP
