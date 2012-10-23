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

#include <string>
#include "mesh.hpp"

class ModelLoader;

class Model {
public:
    friend class ModelLoader;

    Model(const std::string& identifier);

    const std::string& getIdentifier() const;
    size_t getTotalMeshes() const;
    const Mesh& getMesh(const size_t index) const;

    Mesh& mesh(const size_t index);

    void setIdentifier(const std::string& identifier);
    void setTotalMeshes(const size_t totalMeshes);

private:
    std::string m_identifier;
    std::vector<Mesh> m_meshes;
};



inline const std::string& Model::getIdentifier() const {
    return m_identifier;
}

inline size_t Model::getTotalMeshes() const {
    return m_meshes.size();
}

inline const Mesh& Model::getMesh(const size_t index) const {
    return m_meshes[index];
}

inline Mesh& Model::mesh(const size_t index) {
    return m_meshes[index];
}



inline void Model::setIdentifier(const std::string& identifier) {
    m_identifier = identifier;
}

inline void Model::setTotalMeshes(const size_t totalMeshes) {
    m_meshes.resize(totalMeshes);;
}

#endif // MODEL_HPP
