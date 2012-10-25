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


#include "engine/resources/material.hpp"

Material::Material():
    m_colors(),
    m_shininess(0.0f),
    m_textureMaps()
{
    m_colors.resize(TOTAL_MATERIAL_COLORS);
    setColor(MATERIAL_COLOR_DIFFUSE, 0.8f, 0.8f, 0.8f);
    setColor(MATERIAL_COLOR_SPECULAR, 0.0f, 0.0f, 0.0f);
    setColor(MATERIAL_COLOR_AMBIENT, 0.2f, 0.2f, 0.2f);
    setColor(MATERIAL_COLOR_EMISSIVE, 0.0f, 0.0f, 0.0f);
    m_textureMaps.resize(TOTAL_MATERIAL_MAPS);
}
