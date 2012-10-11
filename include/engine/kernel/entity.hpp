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
#include "vector3.hpp"
#include "quaternion.hpp"
#include "commandobject.hpp"
#include "component.hpp"

typedef enum {
    SPACE_LOCAL,
    SPACE_PARENT,
    SPACE_GLOBAL
} transform_space_t;

class Entity: public CommandObject {
public:
    friend class Component;
    friend std::ostream& operator<<(std::ostream& out, const Entity& rhs);

    Entity(const Entity* m_parent, const std::string& objectName);
    ~Entity();

    const Vector3& getPositionAbs() const;
    const Vector3& getPositionRel() const;
    const Quaternion& getOrientationAbs() const;
    const Quaternion& getOrientationRel() const;

    void setPositionAbs(const Vector3& position);
    void setPositionAbs(const scalar_t& posX, const scalar_t& posY, const scalar_t& posZ);
    void setPositionRel(const Vector3& position);
    void setPositionRel(const scalar_t& posX, const scalar_t& posY, const scalar_t& posZ);
    void setOrientationAbs(const Quaternion& orientation);
    void setOrientationAbs(const scalar_t& w, const scalar_t& x, const scalar_t& y, const scalar_t& z);
    void setOrientationAbs(const Vector3& axis, const scalar_t& angle);
    void setOrientationAbs(const scalar_t& yawRad, const scalar_t& pitchRad, const scalar_t& rollRad);
    void setOrientationRel(const Quaternion& orientation);
    void setOrientationRel(const scalar_t& w, const scalar_t& x, const scalar_t& y, const scalar_t& z);
    void setOrientationRel(const Vector3& axis, const scalar_t& angle);
    void setOrientationRel(const scalar_t& yawRad, const scalar_t& pitchRad, const scalar_t& rollRad);

    void translate(const Vector3& displacement, const transform_space_t& relativeTo = SPACE_LOCAL);
    void translate(const scalar_t& distX, const scalar_t& distY, const scalar_t& distZ, const transform_space_t& relativeTo = SPACE_LOCAL);
    void translateX(const scalar_t& distX, const transform_space_t& relativeTo = SPACE_LOCAL);
    void translateY(const scalar_t& distY, const transform_space_t& relativeTo = SPACE_LOCAL);
    void translateZ(const scalar_t& distZ, const transform_space_t& relativeTo = SPACE_LOCAL);

    void rotate(const Quaternion& deltaRotation, const transform_space_t& relativeTo = SPACE_LOCAL);
    void rotate(const scalar_t& w, const scalar_t& x, const scalar_t& y, const scalar_t& z, const transform_space_t& relativeTo = SPACE_LOCAL);
    void rotate(const Vector3& axis, const scalar_t& angle, const transform_space_t& relativeTo = SPACE_LOCAL);
    void yaw(const scalar_t& radians, const transform_space_t& relativeTo = SPACE_LOCAL);
    void pitch(const scalar_t& radians, const transform_space_t& relativeTo = SPACE_LOCAL);
    void roll(const scalar_t& radians, const transform_space_t& relativeTo = SPACE_LOCAL);
    void lookAt(const Vector3& target, const Vector3& up = VECTOR3_UNIT_Y);

    Entity* addChild(const std::string& childName);
    void removeChild(Entity* const child);
    std::string treeToString(const size_t indent) const;

private:
    const Entity& m_parent;
    std::set<Entity*> m_children;
    std::vector<Component*> m_components;
    Vector3 m_positionAbs;
    Vector3 m_positionRel;
    Quaternion m_orientationAbs;
    Quaternion m_orientationRel;
    Quaternion m_lastOrientation;

    void applyTranslationToChildren();
    void applyOrientationToChildren();

    void setPosition(const std::string& arg);
    void setOrientationYPR(const std::string& arg);
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
    void yaw_parent(const std::string& arg);
    void pitch_parent(const std::string& arg);
    void roll_parent(const std::string& arg);
    void yaw_global(const std::string& arg);
    void pitch_global(const std::string& arg);
    void roll_global(const std::string& arg);
};



inline const Vector3& Entity::getPositionAbs() const {
    return m_positionAbs;
}

inline const Vector3& Entity::getPositionRel() const {
    return m_positionRel;
}

inline const Quaternion& Entity::getOrientationAbs() const {
    return m_orientationAbs;
}

inline const Quaternion& Entity::getOrientationRel() const {
    return m_orientationRel;
}



inline void Entity::setPositionAbs(const Vector3& position) {
    m_positionAbs = position;
    m_positionRel = m_positionAbs - m_parent.m_positionAbs;
    applyTranslationToChildren();
}

inline void Entity::setPositionAbs(const scalar_t& posX, const scalar_t& posY, const scalar_t& posZ) {
    setPositionAbs(Vector3(posX, posY, posZ));
}

inline void Entity::setPositionRel(const Vector3& position) {
    m_positionRel = position;
    m_positionAbs = m_positionRel + m_parent.m_positionAbs;
    applyTranslationToChildren();
}

inline void Entity::setPositionRel(const scalar_t& posX, const scalar_t& posY, const scalar_t& posZ) {
    setPositionRel(Vector3(posX, posY, posZ));
}

inline void Entity::setOrientationAbs(const Quaternion& orientation) {
    m_lastOrientation = m_orientationAbs;
    m_orientationAbs = orientation.normalized();
    m_orientationRel = (m_parent.m_orientationAbs.inverse() * m_orientationAbs).normalized();
    applyOrientationToChildren();
}

inline void Entity::setOrientationAbs(const scalar_t& w, const scalar_t& x, const scalar_t& y, const scalar_t& z) {
    setOrientationAbs(Quaternion(w, x, y, z));
}

inline void Entity::setOrientationAbs(const Vector3& axis, const scalar_t& angle) {
    setOrientationAbs(axis, angle);
}

inline void Entity::setOrientationAbs(const scalar_t& yawRad, const scalar_t& pitchRad, const scalar_t& rollRad) {
    setOrientationAbs(Quaternion(yawRad, pitchRad, rollRad));
}

inline void Entity::setOrientationRel(const Quaternion& orientation) {
    m_lastOrientation = m_orientationAbs;
    m_orientationRel = orientation.normalized();
    m_orientationAbs = (m_parent.m_orientationAbs * m_orientationRel).normalized();
    applyOrientationToChildren();
}

inline void Entity::setOrientationRel(const scalar_t& w, const scalar_t& x, const scalar_t& y, const scalar_t& z) {
    setOrientationRel(Quaternion(w, x, y, z));
}

inline void Entity::setOrientationRel(const Vector3& axis, const scalar_t& angle) {
    setOrientationRel(axis, angle);
}

inline void Entity::setOrientationRel(const scalar_t& yawRad, const scalar_t& pitchRad, const scalar_t& rollRad) {
    setOrientationRel(Quaternion(yawRad, pitchRad, rollRad));
}



inline void Entity::translate(const scalar_t& distX, const scalar_t& distY, const scalar_t& distZ, const transform_space_t& relativeTo) {
    translate(Vector3(distX, distY, distZ), relativeTo);
}

inline void Entity::translateX(const scalar_t& distX, const transform_space_t& relativeTo) {
    translate(Vector3(distX, 0, 0), relativeTo);
}

inline void Entity::translateY(const scalar_t& distY, const transform_space_t& relativeTo) {
    translate(Vector3(0, distY, 0), relativeTo);
}

inline void Entity::translateZ(const scalar_t& distZ, const transform_space_t& relativeTo) {
    translate(Vector3(0, 0, distZ), relativeTo);
}

inline void Entity::rotate(const scalar_t& w, const scalar_t& x, const scalar_t& y, const scalar_t& z, const transform_space_t& relativeTo) {
    rotate(Quaternion(w, x, y, z), relativeTo);
}

inline void Entity::rotate(const Vector3& axis, const scalar_t& angle, const transform_space_t& relativeTo) {
    rotate(Quaternion(axis, angle), relativeTo);
}

inline void Entity::yaw(const scalar_t& radians, const transform_space_t& relativeTo) {
    rotate(VECTOR3_UNIT_Y, radians, relativeTo);
}

inline void Entity::pitch(const scalar_t& radians, const transform_space_t& relativeTo) {
    rotate(VECTOR3_UNIT_X, radians, relativeTo);
}

inline void Entity::roll(const scalar_t& radians, const transform_space_t& relativeTo) {
    rotate(VECTOR3_UNIT_Z, radians, relativeTo);
}

#endif // ENTITY_HPP
