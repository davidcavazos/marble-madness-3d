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


#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <string>

class Mesh {
public:
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<unsigned int> indices;
    std::vector<std::vector<float> > uvCoords;

    Mesh();
};

class Model {
public:
    Model(const std::string& identifier);

    const std::string& getIdentifier() const;
    size_t getTotalSubmeshes() const;
    const std::vector<Mesh>& getSubmeshes() const;
    std::vector<Mesh>& getSubmeshes();
    const std::vector<float>& getVertices(const size_t mesh) const;
    const float* getVerticesPtr(const size_t mesh) const;
    const std::vector<float>& getNormals(const size_t mesh) const;
    const float* getNormalsPtr(const size_t mesh) const;
    const std::vector<unsigned int>& getIndices(const size_t mesh) const;
    const unsigned int* getIndicesPtr(const size_t mesh) const;
    const std::vector<float>& getUvCoords(const size_t mesh, const size_t uvMap) const;
    const float* getUvCoordsPtr(const size_t mesh, const size_t uvMap) const;

    void setIdentifier(const std::string& identifier);
    void setTotalSubmeshes(const size_t meshes);
    void setVertices(const size_t mesh, const std::vector<float>& vertices);
    void setVertices(const size_t mesh, const float* vertices, const size_t size);
    void setNormals(const size_t mesh, const std::vector<float>& normals);
    void setNormals(const size_t mesh, const float* normals, const size_t size);
    void setIndices(const size_t mesh, const std::vector<unsigned int>& indices);
    void setIndices(const size_t mesh, const unsigned int* indices, const size_t size);
    void setUvCoords(const size_t mesh, const size_t uvMap, const std::vector<float>& uvCoords);
    void setUvCoords(const size_t mesh, const size_t uvMap, const float* uvCoords, const size_t size);

private:
    std::string m_identifier;
    std::vector<Mesh> m_meshes;
};



inline const std::string& Model::getIdentifier() const {
    return m_identifier;
}

inline size_t Model::getTotalSubmeshes() const {
    return m_meshes.size();
}

inline const std::vector< Mesh >& Model::getSubmeshes() const {
    return m_meshes;
}

inline std::vector< Mesh >& Model::getSubmeshes() {
    return m_meshes;
}

inline const std::vector<float>& Model::getVertices(const size_t mesh) const {
    return m_meshes[mesh].vertices;
}

inline const float* Model::getVerticesPtr(const size_t mesh) const {
    return &m_meshes[mesh].vertices[0];
}

inline const std::vector<float>& Model::getNormals(const size_t mesh) const {
    return m_meshes[mesh].normals;
}

inline const float* Model::getNormalsPtr(const size_t mesh) const {
    return &m_meshes[mesh].normals[0];
}

inline const std::vector<unsigned int>& Model::getIndices(const size_t mesh) const {
    return m_meshes[mesh].indices;
}

inline const unsigned int* Model::getIndicesPtr(const size_t mesh) const {
    return &m_meshes[mesh].indices[0];
}

inline const std::vector<float>& Model::getUvCoords(const size_t mesh, const size_t uvMap) const {
    return m_meshes[mesh].uvCoords[uvMap];
}

inline const float* Model::getUvCoordsPtr(const size_t mesh, const size_t uvMap) const {
    return &m_meshes[mesh].uvCoords[uvMap][0];
}



inline void Model::setIdentifier(const std::string& identifier) {
    m_identifier = identifier;
}

inline void Model::setTotalSubmeshes(const size_t meshes) {
    m_meshes.resize(meshes);
}

inline void Model::setVertices(const size_t mesh, const std::vector<float>& vertices) {
    m_meshes[mesh].vertices = vertices;
}

inline void Model::setVertices(const size_t mesh, const float* vertices, const size_t size) {
    m_meshes[mesh].vertices.resize(size);
    for (size_t i = 0; i < m_meshes[mesh].vertices.size(); ++i)
        m_meshes[mesh].vertices[i] = vertices[i];
}

inline void Model::setNormals(const size_t mesh, const std::vector<float>& normals) {
    m_meshes[mesh].normals = normals;
}

inline void Model::setNormals(const size_t mesh, const float* normals, const size_t size) {
    m_meshes[mesh].normals.resize(size);
    for (size_t i = 0; i < m_meshes[mesh].normals.size(); ++i)
        m_meshes[mesh].normals[i] = normals[i];
}

inline void Model::setIndices(const size_t mesh, const std::vector<unsigned int>& indices) {
    m_meshes[mesh].indices = indices;
}

inline void Model::setIndices(const size_t mesh, const unsigned int* indices, const size_t size) {
    m_meshes[mesh].indices.resize(size);
    for (size_t i = 0; i < m_meshes[mesh].indices.size(); ++i)
        m_meshes[mesh].indices[i] = indices[i];
}

inline void Model::setUvCoords(const size_t mesh, const size_t uvMap, const std::vector<float>& uvCoords) {
    m_meshes[mesh].uvCoords[uvMap] = uvCoords;
}

inline void Model::setUvCoords(const size_t mesh, const size_t uvMap, const float* uvCoords, const size_t size) {
    m_meshes[mesh].uvCoords[uvMap].resize(size);
    for (size_t i = 0; i < m_meshes[mesh].uvCoords[uvMap].size(); ++i)
        m_meshes[mesh].uvCoords[uvMap][i] = uvCoords[i];
}

#endif // MODEL_HPP
