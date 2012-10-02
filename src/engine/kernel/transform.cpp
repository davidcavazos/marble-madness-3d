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

Transform::Transform():
    m_position(0.0f, 0.0f, 0.0f),
    m_rotation(quaternion_t::getIdentity()),
    m_scale(1.0f, 1.0f, 1.0f)
{}

void Transform::rotate(const float yawRad, const float pitchRad, const float rollRad) {
    m_rotation *= quaternion_t(yawRad, pitchRad, rollRad);
}

void Transform::pitch(const float radians) {

}

void Transform::yaw(const float radians) {

}

void Transform::roll(const float radians) {

}

void Transform::scale(const vector3_t multiplier) {

}

void Transform::scale(const float multX, const float multY, const float multZ) {

}

void Transform::scaleX(const float multX) {

}

void Transform::scaleY(const float multY) {

}

void Transform::scaleZ(const float multZ) {

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
