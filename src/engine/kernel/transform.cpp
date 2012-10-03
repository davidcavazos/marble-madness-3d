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


#include "engine/kernel/transform.hpp"

#include <iostream>

using namespace std;

vector3_t Transform::VECTOR_X_AXIS = vector3_t(1.0f, 0.0f, 0.0f);
vector3_t Transform::VECTOR_Y_AXIS = vector3_t(0.0f, 1.0f, 0.0f);
vector3_t Transform::VECTOR_Z_AXIS = vector3_t(0.0f, 0.0f, 1.0f);

Transform::Transform(const Transform& parent):
    m_parent(parent),
    m_position(0.0f, 0.0f, 0.0f),
    m_rotation(quaternion_t::getIdentity())
{}

void Transform::translate(const vector3_t& displacement, const transform_space_t relativeTo) {
    switch (relativeTo) {
    case TS_LOCAL:
        m_position += rotateVector(displacement, m_rotation);
        break;
    case TS_PARENT:
        break;
    case TS_GLOBAL:
        m_position += displacement;
        break;
    default:
        cerr << "Invalid transform_space_t: " << relativeTo << endl;
    }
}

void Transform::translate(const float distX, const float distY, const float distZ, const transform_space_t relativeTo) {
    translate(vector3_t(distX, distY, distZ), relativeTo);
}

void Transform::translateX(const float distX, const transform_space_t relativeTo) {
    translate(vector3_t(distX, 0.0f, 0.0f), relativeTo);
}

void Transform::translateY(const float distY, const transform_space_t relativeTo) {
    translate(vector3_t(0.0f, distY, 0.0f), relativeTo);
}

void Transform::translateZ(const float distZ, const transform_space_t relativeTo) {
    translate(vector3_t(0.0f, 0.0f, distZ), relativeTo);
}

void Transform::rotate(const quaternion_t& rotation, const transform_space_t relativeTo) {
    switch (relativeTo) {
    case TS_LOCAL:
        m_rotation *= rotation;
        break;
    case TS_PARENT:
        break;
    case TS_GLOBAL:
        m_rotation *= rotation;
        break;
    default:
        cerr << "Invalid transform_space_t: " << relativeTo << endl;
    }
}

void Transform::rotate(const float w, const float x, const float y, const float z, const transform_space_t relativeTo) {
    rotate(quaternion_t(w, x, y, z), relativeTo);
}

void Transform::rotate(const float yawRad, const float pitchRad, const float rollRad, const transform_space_t relativeTo) {
    yaw(yawRad, relativeTo);
    pitch(pitchRad, relativeTo);
    roll(rollRad, relativeTo);
}

vector3_t Transform::rotateVector(const vector3_t& v, const quaternion_t& rotation) {
    // nVidia SDK implementation
    vector3_t uv, uuv;
    vector3_t qvec(-rotation.getX(), -rotation.getY(), -rotation.getZ());
    uv = qvec.cross(v);
    uuv = qvec.cross(uv);
    uv *= (2.0f * rotation.getW());
    uuv *= 2.0f;
    return v + uv + uuv;
}

void Transform::pitch(const float radians, const transform_space_t relativeTo) {
    switch (relativeTo) {
    case TS_LOCAL:
        m_rotation *= quaternion_t(VECTOR_X_AXIS, radians);
        break;
    case TS_PARENT:
        break;
    case TS_GLOBAL:
        m_rotation *= quaternion_t(VECTOR_X_AXIS, radians);
        break;
    default:
        cerr << "Invalid transform_space_t: " << relativeTo << endl;
    }
}

void Transform::yaw(const float radians, const transform_space_t relativeTo) {
    switch (relativeTo) {
    case TS_LOCAL:
        m_rotation = quaternion_t(VECTOR_Y_AXIS, radians) * m_rotation;
        break;
    case TS_PARENT:
        break;
    case TS_GLOBAL:
        m_rotation = quaternion_t(VECTOR_Y_AXIS, radians) * m_rotation;
        break;
    default:
        cerr << "Invalid transform_space_t: " << relativeTo << endl;
    }
}

void Transform::roll(const float radians, const transform_space_t relativeTo) {
    switch (relativeTo) {
    case TS_LOCAL:
        m_rotation = quaternion_t(VECTOR_Z_AXIS, radians) * m_rotation;
        break;
    case TS_PARENT:
        break;
    case TS_GLOBAL:
        m_rotation = quaternion_t(VECTOR_Z_AXIS, radians) * m_rotation;
        break;
    default:
        cerr << "Invalid transform_space_t: " << relativeTo << endl;
    }
}

float Transform::calcYaw() const {
    return asin(-2.0 * (m_rotation.x() * m_rotation.z() - m_rotation.w() * m_rotation.y()));
}

float Transform::calcPitch() const {
    return atan2(2.0 * (m_rotation.y() * m_rotation.z() + m_rotation.w() * m_rotation.x()),
                 m_rotation.w() * m_rotation.w() -
                 m_rotation.x() * m_rotation.x() -
                 m_rotation.y() * m_rotation.y() +
                 m_rotation.z() * m_rotation.z());
}

float Transform::calcRoll() const {
    return atan2(2.0 * (m_rotation.x() * m_rotation.y() + m_rotation.w() * m_rotation.z()),
                 m_rotation.w() * m_rotation.w() +
                 m_rotation.x() * m_rotation.x() -
                 m_rotation.y() * m_rotation.y() -
                 m_rotation.z() * m_rotation.z());
}
