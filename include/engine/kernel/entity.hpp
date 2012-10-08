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
#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btQuaternion.h>
#include <bullet/LinearMath/btTransform.h>
#include "commandobject.hpp"
#include "component.hpp"

typedef btVector3 vector3_t;
typedef btQuaternion quaternion_t;
typedef btTransform transform_t;

typedef enum {
    SPACE_LOCAL,
    SPACE_PARENT,
    SPACE_GLOBAL
} transform_space_t;

class Entity: public CommandObject {
public:
    static vector3_t VECTOR_ZERO;
    static vector3_t VECTOR_X_AXIS;
    static vector3_t VECTOR_Y_AXIS;
    static vector3_t VECTOR_Z_AXIS;

    friend class Component;
    friend std::ostream& operator<<(std::ostream& out, const Entity& rhs);

    Entity(const Entity* m_parent, const std::string& objectName);
    ~Entity();

    const vector3_t& getPositionAbs() const;
    const vector3_t& getPositionRel() const;
    const quaternion_t& getRotation() const;

    void setPositionAbs(const vector3_t& position);
    void setPositionAbs(const float posX, const float posY, const float posZ);
    void setPositionRel(const vector3_t& position);
    void setPositionRel(const float posX, const float posY, const float posZ);
    void setRotation(const quaternion_t& rotation);
    void setRotation(const float w, const float x, const float y, const float z);
    void setRotation(const float yawRad, const float pitchRad, const float rollRad);

    void translate(const vector3_t& displacement, const transform_space_t relativeTo = SPACE_LOCAL);
    void translate(const float distX, const float distY, const float distZ, const transform_space_t relativeTo = SPACE_LOCAL);
    void translateX(const float distX, const transform_space_t relativeTo = SPACE_LOCAL);
    void translateY(const float distY, const transform_space_t relativeTo = SPACE_LOCAL);
    void translateZ(const float distZ, const transform_space_t relativeTo = SPACE_LOCAL);

    void rotate(const quaternion_t& rotation, const transform_space_t relativeTo = SPACE_LOCAL);
    void rotate(const float w, const float x, const float y, const float z, const transform_space_t relativeTo = SPACE_LOCAL);
    void yaw(const float radians, const transform_space_t relativeTo = SPACE_LOCAL);
    void pitch(const float radians, const transform_space_t relativeTo = SPACE_LOCAL);
    void roll(const float radians, const transform_space_t relativeTo = SPACE_LOCAL);

    void setDirection(const vector3_t& target);
    void lookAt(const vector3_t& target);
    vector3_t rotateVector(const vector3_t& v, const quaternion_t& rotation);

    void calcOpenGLMatrix(float* m) const;
    float calcYaw() const;
    float calcPitch() const;
    float calcRoll() const;

    Entity* addChild(const std::string& childName);
    void removeChild(Entity* const child);
    std::string treeToString(const size_t indent) const;

private:
    const Entity& m_parent;
    std::set<Entity*> m_children;
    std::vector<Component*> m_components;
    vector3_t m_positionRel;
    vector3_t m_positionAbs;
    quaternion_t m_rotation;
    quaternion_t m_lastRotation;

    void applyTranslationToChildren();
    void applyRotationToChildren();

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
    void yaw_parent(const std::string& arg);
    void pitch_parent(const std::string& arg);
    void roll_parent(const std::string& arg);
    void yaw_global(const std::string& arg);
    void pitch_global(const std::string& arg);
    void roll_global(const std::string& arg);
};



inline const vector3_t& Entity::getPositionAbs() const {
    return m_positionAbs;
}

inline const vector3_t& Entity::getPositionRel() const {
    return m_positionRel;
}

inline const quaternion_t& Entity::getRotation() const {
    return m_rotation;
}

inline void Entity::setPositionAbs(const vector3_t& position) {
    m_positionAbs = position;
    m_positionRel = m_positionAbs - m_parent.m_positionAbs;
    applyTranslationToChildren();
}

inline void Entity::setPositionAbs(const float posX, const float posY, const float posZ) {
    setPositionAbs(vector3_t(posX, posY, posZ));
}

inline void Entity::setPositionRel(const vector3_t& position) {
    m_positionRel = position;
    m_positionAbs = m_positionRel + m_parent.m_positionAbs;
    applyTranslationToChildren();
}

inline void Entity::setPositionRel(const float posX, const float posY, const float posZ) {
    setPositionRel(vector3_t(posX, posY, posZ));
}

inline void Entity::setRotation(const quaternion_t& rotation) {
    m_lastRotation = m_rotation;
    m_rotation = rotation.normalized();
    applyRotationToChildren();
}

inline void Entity::setRotation(const float w, const float x, const float y, const float z) {
    setRotation(quaternion_t(w, x, y, z));
}

inline void Entity::setRotation(const float yawRad, const float pitchRad, const float rollRad) {
    setRotation(quaternion_t(yawRad, pitchRad, rollRad));
}



inline void Entity::translate(const float distX, const float distY, const float distZ, const transform_space_t relativeTo) {
    translate(vector3_t(distX, distY, distZ), relativeTo);
}

inline void Entity::translateX(const float distX, const transform_space_t relativeTo) {
    translate(vector3_t(distX, 0.0f, 0.0f), relativeTo);
}

inline void Entity::translateY(const float distY, const transform_space_t relativeTo) {
    translate(vector3_t(0.0f, distY, 0.0f), relativeTo);
}

inline void Entity::translateZ(const float distZ, const transform_space_t relativeTo) {
    translate(vector3_t(0.0f, 0.0f, distZ), relativeTo);
}

inline void Entity::rotate(const float w, const float x, const float y, const float z, const transform_space_t relativeTo) {
    rotate(quaternion_t(w, x, y, z), relativeTo);
}

inline void Entity::yaw(const float radians, const transform_space_t relativeTo) {
    rotate(quaternion_t(VECTOR_Y_AXIS, radians), relativeTo);
}

inline void Entity::pitch(const float radians, const transform_space_t relativeTo) {
    rotate(quaternion_t(VECTOR_X_AXIS, radians), relativeTo);
}

inline void Entity::roll(const float radians, const transform_space_t relativeTo) {
    rotate(quaternion_t(VECTOR_Z_AXIS, radians), relativeTo);
}

inline void Entity::lookAt(const vector3_t& target) {
    setDirection(target - m_positionAbs);
}

#endif // ENTITY_HPP
