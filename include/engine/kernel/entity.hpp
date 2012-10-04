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
#include "transform.hpp"

class Entity: public CommandObject {
public:
    friend class Component;
    friend std::ostream& operator<<(std::ostream& out, const Entity& rhs);

    Entity(const Entity* m_parent, const std::string& objectName);
    ~Entity();

    const Transform& getTransform() const;
    Transform& getTransform();

    Entity* addChild(const std::string& childName);
    void removeChild(Entity* const child);
    std::string treeToString(const size_t indent) const;

private:
    const Entity& m_parent;
    std::set<Entity*> m_children;
    std::vector<Component*> m_components;
    Transform m_transform;

    void setPosition(const std::string& arg);
    void moveXYZ(const std::string& arg);
    void moveX(const std::string& arg);
    void moveY(const std::string& arg);
    void moveZ(const std::string& arg);
    void moveXYZ_parent(const std::string& arg);
    void moveX_parent(const std::string& arg);
    void moveY_parent(const std::string& arg);
    void moveZ_parent(const std::string& arg);
    void moveXYZ_global(const std::string& arg);
    void moveX_global(const std::string& arg);
    void moveY_global(const std::string& arg);
    void moveZ_global(const std::string& arg);
    void yaw(const std::string& arg);
    void pitch(const std::string& arg);
    void roll(const std::string& arg);
    void yaw_global(const std::string& arg);
    void pitch_global(const std::string& arg);
    void roll_global(const std::string& arg);
};



inline const Transform& Entity::getTransform() const {
    return m_transform;
}

inline Transform& Entity::getTransform() {
    return m_transform;
}

#endif // ENTITY_HPP
