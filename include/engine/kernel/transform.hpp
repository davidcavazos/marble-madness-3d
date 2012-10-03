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


#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btQuaternion.h>
#include <bullet/LinearMath/btTransform.h>

typedef btVector3 vector3_t;
typedef btQuaternion quaternion_t;
typedef btTransform transform_t;

typedef enum {
    TS_LOCAL,
    TS_PARENT,
    TS_GLOBAL
} transform_space_t;

class Transform {
public:
    static vector3_t VECTOR_X_AXIS;
    static vector3_t VECTOR_Y_AXIS;
    static vector3_t VECTOR_Z_AXIS;

    Transform(const Transform& parent);

    const vector3_t& getPosition() const;
    const quaternion_t& getRotation() const;

    void setPosition(const vector3_t& position);
    void setPosition(const float posX, const float posY, const float posZ);

    void setRotation(const quaternion_t& rotation);
    void setRotation(const float w, const float x, const float y, const float z);
    void setRotation(const float yawRad, const float pitchRad, const float rollRad);

    void translate(const vector3_t& displacement, const transform_space_t relativeTo = TS_LOCAL);
    void translate(const float distX, const float distY, const float distZ, const transform_space_t relativeTo = TS_LOCAL);
    void translateX(const float distX, const transform_space_t relativeTo = TS_LOCAL);
    void translateY(const float distY, const transform_space_t relativeTo = TS_LOCAL);
    void translateZ(const float distZ, const transform_space_t relativeTo = TS_LOCAL);

    void rotate(const quaternion_t& rotation);
    void rotate(const float w, const float x, const float y, const float z);
    void pitch(const float radians);
    void yaw(const float radians);
    void roll(const float radians);

    vector3_t rotateVector(const vector3_t& v, const quaternion_t& rotation);

    float calcYaw() const;
    float calcPitch() const;
    float calcRoll() const;

private:
    const Transform& m_parent;
    vector3_t m_position;
    quaternion_t m_rotation;
};



inline const vector3_t& Transform::getPosition() const {
    return m_position;
}

inline const quaternion_t& Transform::getRotation() const {
    return m_rotation;
}

inline void Transform::setPosition(const vector3_t& position) {
    m_position = position;
}

inline void Transform::setPosition(const float posX, const float posY, const float posZ) {
    setPosition(vector3_t(posX, posY, posZ));
}

inline void Transform::setRotation(const quaternion_t& rotation) {
    m_rotation = rotation;
}

inline void Transform::setRotation(const float w, const float x, const float y, const float z) {
    setRotation(quaternion_t(w, x, y, z));
}

inline void Transform::setRotation(const float yawRad, const float pitchRad, const float rollRad) {
    setRotation(quaternion_t(yawRad, pitchRad, rollRad));
}



inline void Transform::translate(const float distX, const float distY, const float distZ, const transform_space_t relativeTo) {
    translate(vector3_t(distX, distY, distZ), relativeTo);
}

inline void Transform::translateX(const float distX, const transform_space_t relativeTo) {
    translate(vector3_t(distX, 0.0f, 0.0f), relativeTo);
}

inline void Transform::translateY(const float distY, const transform_space_t relativeTo) {
    translate(vector3_t(0.0f, distY, 0.0f), relativeTo);
}

inline void Transform::translateZ(const float distZ, const transform_space_t relativeTo) {
    translate(vector3_t(0.0f, 0.0f, distZ), relativeTo);
}

inline void Transform::rotate(const quaternion_t& rotation) {
    m_rotation *= rotation;
}

inline void Transform::rotate(const float w, const float x, const float y, const float z) {
    rotate(quaternion_t(w, x, y, z));
}

inline void Transform::pitch(const float radians) {
    rotate(quaternion_t(VECTOR_X_AXIS, radians));
}

inline void Transform::yaw(const float radians) {
    rotate(quaternion_t(VECTOR_Y_AXIS, radians));
}

inline void Transform::roll(const float radians) {
    rotate(quaternion_t(VECTOR_Z_AXIS, radians));
}

#endif // TRANSFORM_HPP
