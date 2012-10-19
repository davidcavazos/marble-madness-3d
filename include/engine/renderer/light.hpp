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


#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "engine/kernel/component.hpp"

class Light: public Component {
public:
    Light(Entity*const entity);
    ~Light();

    const float* getAmbient() const;
    const float* getDiffuse() const;
    const float* getSpecular() const;
    void setAmbient(const float r, const float g, const float b, const float a = 1.0f);
    void setDiffuse(const float r, const float g, const float b, const float a = 1.0f);
    void setSpecular(const float r, const float g, const float b, const float a = 1.0f);

private:
    float m_ambient[4];
    float m_diffuse[4];
    float m_specular[4];
};



inline const float* Light::getAmbient() const {
    return m_ambient;
}

inline const float* Light::getDiffuse() const {
    return m_diffuse;
}

inline const float* Light::getSpecular() const {
    return m_specular;
}

inline void Light::setAmbient(const float r, const float g, const float b, const float a) {
    m_ambient[0] = r;
    m_ambient[1] = g;
    m_ambient[2] = b;
    m_ambient[3] = a;
}

inline void Light::setDiffuse(const float r, const float g, const float b, const float a) {
    m_diffuse[0] = r;
    m_diffuse[1] = g;
    m_diffuse[2] = b;
    m_diffuse[3] = a;
}

inline void Light::setSpecular(const float r, const float g, const float b, const float a) {
    m_specular[0] = r;
    m_specular[1] = g;
    m_specular[2] = b;
    m_specular[3] = a;
}

#endif // LIGHT_HPP
