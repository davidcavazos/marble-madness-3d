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
#include <set>
#include "engine/kernel/entity.hpp"

using namespace std;

vector3_t Transform::VECTOR_ZERO = vector3_t(0.0f, 0.0f, 0.0f);
vector3_t Transform::VECTOR_X_AXIS = vector3_t(1.0f, 0.0f, 0.0f);
vector3_t Transform::VECTOR_Y_AXIS = vector3_t(0.0f, 1.0f, 0.0f);
vector3_t Transform::VECTOR_Z_AXIS = vector3_t(0.0f, 0.0f, 1.0f);

Transform::Transform(const Entity& entity, const Transform& parent):
    m_entity(entity),
    m_parent(parent),
    m_positionRel(0.0f, 0.0f, 0.0f),
    m_positionAbs(0.0f, 0.0f, 0.0f),
    m_rotation(quaternion_t::getIdentity()),
    m_lastRotation(quaternion_t::getIdentity())
{}

void Transform::translate(const vector3_t& displacement, const transform_space_t relativeTo) {
    switch (relativeTo) {
    case SPACE_LOCAL:
//         setPositionRel(m_positionRel + displacement.rotate(m_rotation.getAxis(), m_rotation.getAngle()));
        setPositionRel(m_positionRel + rotateVector(displacement, m_rotation));
        break;
    case SPACE_PARENT:
//         setPositionRel(m_positionRel + displacement.rotate(m_parent.m_rotation.getAxis(), m_parent.m_rotation.getAngle()));
        setPositionRel(m_positionRel + rotateVector(displacement, m_parent.m_rotation));
        break;
    case SPACE_GLOBAL:
        setPositionAbs(m_positionAbs + displacement);
        break;
    default:
        cerr << "Invalid transform_space_t: " << relativeTo << endl;
    }
}

void Transform::rotate(const quaternion_t& rotation, const transform_space_t relativeTo) {
    switch (relativeTo) {
    case SPACE_LOCAL:
        setRotation(m_rotation * rotation);
        break;
    case SPACE_PARENT:
        setRotation(rotation * m_parent.m_rotation * m_rotation);
        break;
    case SPACE_GLOBAL:
        setRotation(rotation * m_rotation);
        break;
    default:
        cerr << "Invalid transform_space_t: " << relativeTo << endl;
    }
}

void Transform::setDirection(const vector3_t& target) {
    if (target == VECTOR_ZERO)
        return;
    cerr << "Transform.setDirection(vector3) not implemented yet!" << endl;
}

vector3_t Transform::rotateVector(const vector3_t& v, const quaternion_t& rotation) {
//     // Mathematical method
//     quaternion_t vecQuat(v.getX(), v.getY(), v.getZ(), 0.0f);
//     quaternion_t resQuat = rotation * vecQuat * rotation.inverse();
//     return vector3_t(resQuat.getX(), resQuat.getY(), resQuat.getZ());

    // nVidia SDK implementation
    vector3_t uv, uuv;
    vector3_t qvec(rotation.getX(), rotation.getY(), rotation.getZ());
    uv = qvec.cross(v);
    uuv = qvec.cross(uv);
    uv *= (2.0f * rotation.getW());
    uuv *= 2.0f;
    return v + uv + uuv;
}

void Transform::calcOpenGLMatrix(float* m) const {
    transform_t t(m_rotation, m_positionAbs);
    t.getOpenGLMatrix(m);
}

float Transform::calcYaw() const {
    return asin(-2.0 * (m_rotation.getX() * m_rotation.getZ() - m_rotation.getW() * m_rotation.getY()));
}

float Transform::calcPitch() const {
    return atan2(2.0 * (m_rotation.getY() * m_rotation.getZ() + m_rotation.getW() * m_rotation.getX()),
                 m_rotation.getW() * m_rotation.getW() -
                 m_rotation.getX() * m_rotation.getX() -
                 m_rotation.getY() * m_rotation.getY() +
                 m_rotation.getZ() * m_rotation.getZ());
}

float Transform::calcRoll() const {
    return atan2(2.0 * (m_rotation.getX() * m_rotation.getY() + m_rotation.getW() * m_rotation.getZ()),
                 m_rotation.getW() * m_rotation.getW() +
                 m_rotation.getX() * m_rotation.getX() -
                 m_rotation.getY() * m_rotation.getY() -
                 m_rotation.getZ() * m_rotation.getZ());
}

void Transform::applyTranslationToChildren() {
    set<Entity*>::iterator it, itend;
    itend = m_entity.m_children.end();
    for (it = m_entity.m_children.begin(); it != itend; ++it) {
        Transform& child = (*it)->m_transform;
        child.setPositionRel(child.m_positionRel);
    }
}

void Transform::applyRotationToChildren() {
    set<Entity*>::iterator it, itend;
    itend = m_entity.m_children.end();
    for (it = m_entity.m_children.begin(); it != itend; ++it) {
        Transform& child = (*it)->m_transform;
        quaternion_t relativeRotation = m_rotation * m_lastRotation.inverse();
        child.setPositionRel(rotateVector(child.m_positionRel, relativeRotation));
        child.setRotation(relativeRotation * child.m_rotation);
    }
}
