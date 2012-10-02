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

class Transform {
public:
    Transform();

    const vector3_t& getPosition() const;
    const quaternion_t& getRotation() const;
    const vector3_t& getScale() const;

    void setPosition(const vector3_t& position);
    void setPosition(const float posX, const float posY, const float posZ);

    void setRotation(const quaternion_t& rotation);
    void setRotation(const float w, const float x, const float y, const float z);
    void setRotation(const float yawRad, const float pitchRad, const float rollRad);

    void setScale(const vector3_t scale);
    void setScale(const float scaleX, const float scaleY, const float scaleZ);


    void translate(const vector3_t distance);
    void translate(const float distX, const float distY, const float distZ);
    void translateX(const float distX);
    void translateY(const float distY);
    void translateZ(const float distZ);

    void rotate(const quaternion_t& rotation);
    void rotate(const float w, const float x, const float y, const float z);
    void rotate(const float yawRad, const float pitchRad, const float rollRad);
    void pitch(const float radians);
    void yaw(const float radians);
    void roll(const float radians);

    void scale(const vector3_t multiplier);
    void scale(const float multX, const float multY, const float multZ);
    void scaleX(const float multX);
    void scaleY(const float multY);
    void scaleZ(const float multZ);

    float calcYaw() const;
    float calcPitch() const;
    float calcRoll() const;

private:
    vector3_t m_position;
    quaternion_t m_rotation;
    vector3_t m_scale;
};



inline const vector3_t& Transform::getPosition() const {
    return m_position;
}

inline const quaternion_t& Transform::getRotation() const {
    return m_rotation;
}

inline const vector3_t& Transform::getScale() const {
    return m_scale;
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

inline void Transform::setScale(const vector3_t scale) {
    m_scale = scale;
}

inline void Transform::setScale(const float scaleX, const float scaleY, const float scaleZ) {
    setScale(vector3_t(scaleX, scaleY, scaleZ));
}



inline void Transform::translate(const vector3_t distance) {
    m_position += distance;
}

inline void Transform::translate(const float distX, const float distY, const float distZ) {
    translate(vector3_t(distX, distY, distZ));
}

inline void Transform::translateX(const float distX) {
    m_position.setX(m_position.getX() + distX);
}

inline void Transform::translateY(const float distY) {
    m_position.setY(m_position.getY() + distY);
}

inline void Transform::translateZ(const float distZ) {
    m_position.setZ(m_position.getZ() + distZ);
}

inline void Transform::rotate(const quaternion_t& rotation) {
    m_rotation *= rotation;
}

inline void Transform::rotate(const float w, const float x, const float y, const float z) {
    rotate(quaternion_t(w, x, y, z));
}

#endif // TRANSFORM_HPP
