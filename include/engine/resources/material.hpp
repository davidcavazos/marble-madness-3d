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


#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <iostream>
#include <vector>
#include <cstddef>

typedef enum {
    MATERIAL_DIFFUSE_MAP,
    MATERIAL_AMBIENT_MAP,
    MATERIAL_EMISSIVE_MAP,
    MATERIAL_HEIGHT_MAP,
    MATERIAL_NORMALS_MAP,
    MATERIAL_SHININESS_MAP,
    MATERIAL_OPACITY_MAP,
    MATERIAL_DISPLACEMENT_MAP,
    MATERIAL_LIGHT_MAP,
    MATERIAL_REFLECTION_MAP,
    TOTAL_MATERIAL_MAPS
} material_map_t;

class Material {
public:
    std::vector<float> m_uvCoords;

    Material();

    const std::vector<float>& getUvCoords() const;
    const float* getUvCoordsPtr() const;
    size_t getTotalUvCoords() const;
    size_t getMapIndex(const material_map_t& materialMapType) const;

    void setUvCoords(const std::vector<float>& uvCoords);
    void setUvCoords(const float* uvCoords, const size_t size);
    void setMapIndex(const material_map_t& materialMapType, const size_t index);

private:
    std::vector<size_t> m_materialMapIndex;
};



inline const std::vector< float >& Material::getUvCoords() const {
    return m_uvCoords;
}

inline const float* Material::getUvCoordsPtr() const {
    return &m_uvCoords[0];
}

inline size_t Material::getTotalUvCoords() const {
    return m_uvCoords.size();
}

inline size_t Material::getMapIndex(const material_map_t& materialMapType) const {
    return m_materialMapIndex[materialMapType];
}



inline void Material::setUvCoords(const std::vector< float >& uvCoords) {
    m_uvCoords = uvCoords;
}

inline void Material::setUvCoords(const float* uvCoords, const size_t size) {
    m_uvCoords.resize(size);
    for (size_t i = 0; i < size; ++i)
        m_uvCoords[i] = uvCoords[i];
}

inline void Material::setMapIndex(const material_map_t& materialMapType, const size_t index) {
    m_materialMapIndex[materialMapType] = index;
}

#endif // MATERIAL_HPP
