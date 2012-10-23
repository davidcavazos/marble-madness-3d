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
    MATERIAL_COLOR_DIFFUSE,
    MATERIAL_COLOR_SPECULAR,
    MATERIAL_COLOR_AMBIENT,
    MATERIAL_COLOR_EMISSIVE,
    MATERIAL_COLOR_TRANSPARENT,
    TOTAL_MATERIAL_COLORS
} material_color_t;

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

typedef struct {
    float rgba[4];
} color_t;

class Material {
public:
    Material();

    const color_t& getColor(const material_color_t& materialColorType) const;
    size_t getMapIndex(const material_map_t& materialMapType) const;

    void setColor(const material_color_t& materialColorType, const color_t& rgba);
    void setColor(const material_color_t& materialColorType, const float r, const float g, const float b, const float a);
    void setMapIndex(const material_map_t& materialMapType, const size_t index);

private:
    std::vector<color_t> m_colors;
    std::vector<size_t> m_mapIndices;
};



inline const color_t& Material::getColor(const material_color_t& materialColorType) const {
    return m_colors[materialColorType];
}

inline size_t Material::getMapIndex(const material_map_t& materialMapType) const {
    return m_mapIndices[materialMapType];
}



inline void Material::setColor(const material_color_t& materialColorType, const color_t& rgba) {
    m_colors[materialColorType] = rgba;
}

inline void Material::setColor(const material_color_t& materialColorType, const float r, const float g, const float b, const float a) {
    m_colors[materialColorType].rgba[0] = r;
    m_colors[materialColorType].rgba[1] = g;
    m_colors[materialColorType].rgba[2] = b;
    m_colors[materialColorType].rgba[3] = a;
}

inline void Material::setMapIndex(const material_map_t& materialMapType, const size_t index) {
    m_mapIndices[materialMapType] = index;
}

#endif // MATERIAL_HPP
