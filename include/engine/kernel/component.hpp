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


#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <string>

class Entity;

typedef enum {
    COMPONENT_CAMERA,
    COMPONENT_LIGHT,
    COMPONENT_RENDERABLE_MESH,
    COMPONENT_PHYSICS,
    TOTAL_COMPONENTS_CONTAINER_SIZE // Do not use as a component, gives size to component container
} component_t;

class Component {
public:
    Component(const component_t type, Entity* const entity);
    virtual ~Component();

    Entity& getEntity();
    component_t getType() const;
    const std::string& getDescription() const;

protected:
    Entity& m_entity;
    const component_t m_type;
    std::string m_description;
};

std::ostream& operator<<(std::ostream& out, const Component& rhs);



inline Entity& Component::getEntity() {
    return m_entity;
}

inline component_t Component::getType() const {
    return m_type;
}

inline const std::string& Component::getDescription() const {
    return m_description;
}

#endif // COMPONENT_HPP
