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


#include "engine/kernel/entity.hpp"

#include <iostream>
#include <cmath>
#include "engine/kernel/matrix3x3.hpp"
#include "engine/kernel/devicemanager.hpp"
#include "engine/kernel/scenemanager.hpp"
#include "engine/kernel/component.hpp"
#include "engine/physics/rigidbody.hpp"

using namespace std;

const size_t INDENT_SIZE = 2;

Entity::Entity(const Entity* parent, const string& objectName):
    CommandObject(objectName),
    m_parent(*parent),
    m_children(),
    m_components(TOTAL_COMPONENTS_CONTAINER_SIZE, 0),
    m_positionAbs(VECTOR3_ZERO),
    m_positionRel(VECTOR3_ZERO),
    m_orientationAbs(QUATERNION_IDENTITY),
    m_orientationRel(QUATERNION_IDENTITY),
    m_lastOrientation(QUATERNION_IDENTITY)
{
    if (&m_parent != 0) {
        setPositionRel(VECTOR3_ZERO);
        setOrientationRel(QUATERNION_IDENTITY);
    }
    registerAttribute("position-abs", boost::bind(&Entity::cmdPositionAbs, this, _1));
    registerAttribute("position-rel", boost::bind(&Entity::cmdPositionRel, this, _1));
    registerAttribute("orientation-abs-ypr", boost::bind(&Entity::cmdOrientationAbsYPR, this, _1));
    registerAttribute("orientation-rel-ypr", boost::bind(&Entity::cmdOrientationRelYPR, this, _1));
    registerCommand("move-xyz", boost::bind(&Entity::cmdMoveXYZ, this, _1));
    registerCommand("move-x", boost::bind(&Entity::cmdMoveX, this, _1));
    registerCommand("move-y", boost::bind(&Entity::cmdMoveY, this, _1));
    registerCommand("move-z", boost::bind(&Entity::cmdMoveZ, this, _1));
    registerCommand("move-xyz-parent", boost::bind(&Entity::cmdMoveXYZ_parent, this, _1));
    registerCommand("move-x-parent", boost::bind(&Entity::cmdMoveX_parent, this, _1));
    registerCommand("move-y-parent", boost::bind(&Entity::cmdMoveY_parent, this, _1));
    registerCommand("move-z-parent", boost::bind(&Entity::cmdMoveZ_parent, this, _1));
    registerCommand("move-xyz-global", boost::bind(&Entity::cmdMoveXYZ_global, this, _1));
    registerCommand("move-x-global", boost::bind(&Entity::cmdMoveX_global, this, _1));
    registerCommand("move-y-global", boost::bind(&Entity::cmdMoveY_global, this, _1));
    registerCommand("move-z-global", boost::bind(&Entity::cmdMoveZ_global, this, _1));
    registerCommand("yaw", boost::bind(&Entity::cmdYaw, this, _1));
    registerCommand("pitch", boost::bind(&Entity::cmdPitch, this, _1));
    registerCommand("roll", boost::bind(&Entity::cmdRoll, this, _1));
    registerCommand("yaw-parent", boost::bind(&Entity::cmdYaw_parent, this, _1));
    registerCommand("pitch-parent", boost::bind(&Entity::cmdPitch_parent, this, _1));
    registerCommand("roll-parent", boost::bind(&Entity::cmdRoll_parent, this, _1));
    registerCommand("yaw-global", boost::bind(&Entity::cmdYaw_global, this, _1));
    registerCommand("pitch-global", boost::bind(&Entity::cmdPitch_global, this, _1));
    registerCommand("roll-global", boost::bind(&Entity::cmdRoll_global, this, _1));
}

Entity::~Entity() {
    set<Entity*>::iterator it;
    for (it = m_children.begin(); it != m_children.end(); ++it)
        delete *it;
    for (size_t i = 0; i < m_components.size(); ++i)
        delete m_components[i];
}


void Entity::translate(const Vector3& displacement, const transform_space_t& relativeTo) {
    switch (relativeTo) {
    case SPACE_LOCAL:
        setPositionRel(m_positionRel + displacement.rotate(m_orientationAbs));
        break;
    case SPACE_PARENT:
        setPositionRel(m_positionRel + displacement.rotate(m_parent.m_orientationAbs));
        break;
    case SPACE_GLOBAL:
        setPositionAbs(m_positionAbs + displacement);
        break;
    default:
        cerr << "Invalid transform_space_t: " << relativeTo << endl;
    }
}

void Entity::rotate(const Quaternion& deltaRotation, const transform_space_t& relativeTo) {
    switch (relativeTo) {
    case SPACE_LOCAL:
        setOrientationAbs(m_orientationAbs * deltaRotation);
        break;
    case SPACE_PARENT:
        setOrientationRel(deltaRotation * m_orientationRel);
        break;
    case SPACE_GLOBAL:
        setOrientationAbs(deltaRotation * m_orientationAbs);
        break;
    default:
        cerr << "Invalid transform_space_t: " << relativeTo << endl;
    }
}

void Entity::lookAt(const Vector3& target, const Vector3& up) {
    Quaternion result;
    Vector3 vFwd = (target - m_positionAbs).normalized();
    Vector3 vSide = vFwd.cross(up).normalized();
    Vector3 vUp = vSide.cross(vFwd);
    Matrix3x3 m(vSide.getX(), vUp.getX(), -vFwd.getX(),
                vSide.getY(), vUp.getY(), -vFwd.getY(),
                vSide.getZ(), vUp.getZ(), -vFwd.getZ());
    m.getRotation(result);
    setOrientationAbs(result);
}

void Entity::applyTranslationToChildren() {
    set<Entity*>::iterator it, itend;
    itend = m_children.end();
    for (it = m_children.begin(); it != itend; ++it) {
        Entity& child = **it;
        child.setPositionRel(child.m_positionRel);
    }
}

void Entity::applyOrientationToChildren() {
    // Qc1 = Qp1 * Inv(Qp0) * Qc0
    // Where:
    // Qp0 = Parent orientation last frame
    // Qp1 = Parent orientation this frame
    // Qc0 = Child orientation last frame
    // Qc1 = Child orientation this frame
    Quaternion relativeRotation = m_orientationAbs * m_lastOrientation.inverse();
    set<Entity*>::iterator it, itend;
    itend = m_children.end();
    for (it = m_children.begin(); it != itend; ++it) {
        Entity& child = **it;
        child.setPositionRel(child.m_positionRel.rotate(relativeRotation));
        child.setOrientationAbs(relativeRotation * child.m_orientationAbs);
    }
}

void Entity::applyTransformToPhysicsComponent() {
    if (m_components[COMPONENT_PHYSICS] != 0)
        dynamic_cast<RigidBody*>(m_components[COMPONENT_PHYSICS])->setTransform(m_positionAbs, m_orientationAbs);
}

Entity* Entity::addChild(const string& childName) {
    Entity* child = new Entity(this, childName);
    m_children.insert(child);
    SceneManager::ms_entities.insert(pair<string, Entity*>(childName, child));
    return child;
}

void Entity::removeChild(Entity* const child) {
    map<string, Entity*>::iterator smIt = SceneManager::ms_entities.find(child->getObjectName());
    if (smIt != SceneManager::ms_entities.end())
        SceneManager::ms_entities.erase(smIt);

    set<Entity*>::iterator it = m_children.find(child);
    if (it != m_children.end()) {
        delete *it;
        m_children.erase(it);
    }
}

string Entity::treeToString(const size_t indent) const {
    stringstream ss;
    for (size_t i = 0; i < indent; ++i)
        ss << " ";
    ss << m_objectName << endl;
    set<Entity*>::iterator it;
    for (it = m_children.begin(); it != m_children.end(); ++it)
        ss << (*it)->treeToString(indent + INDENT_SIZE);
    return ss.str();
}



void Entity::cmdPositionAbs(const string& arg) {
    scalar_t x, y, z;
    stringstream ss(arg);
    ss >> x >> y >> z;
    setPositionAbs(x, y, z);
}

void Entity::cmdPositionRel(const string& arg) {
    scalar_t x, y, z;
    stringstream ss(arg);
    ss >> x >> y >> z;
    setPositionRel(x, y, z);
}

void Entity::cmdOrientationAbsYPR(const string& arg) {
    scalar_t yaw, pitch, roll;
    stringstream ss(arg);
    ss >> yaw >> pitch >> roll;
    setOrientationAbs(yaw, pitch, roll);
}

void Entity::cmdOrientationRelYPR(const string& arg) {
    scalar_t yaw, pitch, roll;
    stringstream ss(arg);
    ss >> yaw >> pitch >> roll;
    setOrientationRel(yaw, pitch, roll);
}

void Entity::cmdMoveXYZ(const std::string& arg) {
    scalar_t x, y, z;
    stringstream ss(arg);
    ss >> x >> y >> z;
    translate(x, y, z);
}

void Entity::cmdMoveX(const std::string& arg) {
    scalar_t dist;
    stringstream ss(arg);
    ss >> dist;
    translateX(dist * DeviceManager::getDeltaTime());
}

void Entity::cmdMoveY(const std::string& arg) {
    scalar_t dist;
    stringstream ss(arg);
    ss >> dist;
    translateY(dist * DeviceManager::getDeltaTime());
}

void Entity::cmdMoveZ(const std::string& arg) {
    scalar_t dist;
    stringstream ss(arg);
    ss >> dist;
    translateZ(dist * DeviceManager::getDeltaTime());
}

void Entity::cmdMoveXYZ_parent(const std::string& arg) {
    scalar_t x, y, z;
    stringstream ss(arg);
    ss >> x >> y >> z;
    translate(
        x * DeviceManager::getDeltaTime(),
        y * DeviceManager::getDeltaTime(),
        z * DeviceManager::getDeltaTime(),
        SPACE_PARENT
    );
}

void Entity::cmdMoveX_parent(const std::string& arg) {
    scalar_t dist;
    stringstream ss(arg);
    ss >> dist;
    translateX(dist * DeviceManager::getDeltaTime(), SPACE_PARENT);
}

void Entity::cmdMoveY_parent(const std::string& arg) {
    scalar_t dist;
    stringstream ss(arg);
    ss >> dist;
    translateY(dist * DeviceManager::getDeltaTime(), SPACE_PARENT);
}

void Entity::cmdMoveZ_parent(const std::string& arg) {
    scalar_t dist;
    stringstream ss(arg);
    ss >> dist;
    translateZ(dist * DeviceManager::getDeltaTime(), SPACE_PARENT);
}

void Entity::cmdMoveXYZ_global(const std::string& arg) {
    scalar_t x, y, z;
    stringstream ss(arg);
    ss >> x >> y >> z;
    translate(
        x * DeviceManager::getDeltaTime(),
        y * DeviceManager::getDeltaTime(),
        z * DeviceManager::getDeltaTime(),
        SPACE_GLOBAL
    );
}

void Entity::cmdMoveX_global(const std::string& arg) {
    scalar_t dist;
    stringstream ss(arg);
    ss >> dist;
    translateX(dist * DeviceManager::getDeltaTime(), SPACE_GLOBAL);
}

void Entity::cmdMoveY_global(const std::string& arg) {
    scalar_t dist;
    stringstream ss(arg);
    ss >> dist;
    translateY(dist * DeviceManager::getDeltaTime(), SPACE_GLOBAL);
}

void Entity::cmdMoveZ_global(const std::string& arg) {
    scalar_t dist;
    stringstream ss(arg);
    ss >> dist;
    translateZ(dist * DeviceManager::getDeltaTime(), SPACE_GLOBAL);
}

void Entity::cmdYaw(const std::string& arg) {
    scalar_t radians;
    stringstream ss(arg);
    ss >> radians;
    yaw(radians * DeviceManager::getDeltaTime());
}

void Entity::cmdPitch(const std::string& arg) {
    scalar_t radians;
    stringstream ss(arg);
    ss >> radians;
    pitch(radians * DeviceManager::getDeltaTime());
}

void Entity::cmdRoll(const std::string& arg) {
    scalar_t radians;
    stringstream ss(arg);
    ss >> radians;
    roll(radians * DeviceManager::getDeltaTime());
}

void Entity::cmdYaw_parent(const std::string& arg) {
    scalar_t radians;
    stringstream ss(arg);
    ss >> radians;
    yaw(radians * DeviceManager::getDeltaTime(), SPACE_PARENT);
}

void Entity::cmdPitch_parent(const std::string& arg) {
    scalar_t radians;
    stringstream ss(arg);
    ss >> radians;
    pitch(radians * DeviceManager::getDeltaTime(), SPACE_PARENT);
}

void Entity::cmdRoll_parent(const std::string& arg) {
    scalar_t radians;
    stringstream ss(arg);
    ss >> radians;
    roll(radians * DeviceManager::getDeltaTime(), SPACE_PARENT);
}

void Entity::cmdYaw_global(const std::string& arg) {
    scalar_t radians;
    stringstream ss(arg);
    ss >> radians;
    yaw(radians * DeviceManager::getDeltaTime(), SPACE_GLOBAL);
}

void Entity::cmdPitch_global(const std::string& arg) {
    scalar_t radians;
    stringstream ss(arg);
    ss >> radians;
    pitch(radians * DeviceManager::getDeltaTime(), SPACE_GLOBAL);
}

void Entity::cmdRoll_global(const std::string& arg) {
    scalar_t radians;
    stringstream ss(arg);
    ss >> radians;
    roll(radians * DeviceManager::getDeltaTime(), SPACE_GLOBAL);
}

ostream& operator<<(ostream& out, const Entity& rhs) {
    out << "position(" << rhs.getPositionAbs().getX() << ", " <<
            rhs.getPositionAbs().getY() << ", " <<
            rhs.getPositionAbs().getZ() << ")" << endl;

    out << "rotation(" << rhs.getOrientationAbs().getW() << ", " <<
            rhs.getOrientationAbs().getX() << ", " <<
            rhs.getOrientationAbs().getY() << ", " <<
            rhs.getOrientationAbs().getZ() << ")" << endl;

    for (size_t i = 0; i < rhs.m_components.size(); ++i) {
        if (rhs.m_components[i] != 0)
            out << "    " << *rhs.m_components[i] << endl;
    }
    return out;
}
