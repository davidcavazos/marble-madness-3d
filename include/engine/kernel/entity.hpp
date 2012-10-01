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


#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <ostream>
#include <vector>
#include <set>
#include "commandobject.hpp"
#include "component.hpp"

class Entity: public CommandObject {
public:
    friend class Component;
    friend std::ostream& operator<<(std::ostream& out, const Entity& rhs);

    Entity(const Entity* m_parent, const std::string& objectName);
    ~Entity();

    double getPositionX() const;
    double getPositionY() const;
    double getPositionZ() const;
    void setPosition(const double x, const double y, const double z);

    Entity* addChild(const std::string& childName);
    void removeChild(Entity* const child);
    std::string treeToString(const size_t indent) const;

private:
    const Entity& m_parent;
    std::set<Entity*> m_children;
    std::vector<Component*> m_components;
    double m_positionX;
    double m_positionY;
    double m_positionZ;

    void position(const std::string& arg);
    void moveX(const std::string& arg);
    void moveY(const std::string& arg);
    void moveZ(const std::string& arg);
};



inline double Entity::getPositionX() const {
    return m_positionX;
}

inline double Entity::getPositionY() const {
    return m_positionY;
}

inline double Entity::getPositionZ() const {
    return m_positionZ;
}

inline void Entity::setPosition(const double x, const double y, const double z) {
    m_positionX = x;
    m_positionY = y;
    m_positionZ = z;
}

#endif // ENTITY_HPP
