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

#include "engine/kernel/devicemanager.hpp"
#include "engine/kernel/scenemanager.hpp"
#include "engine/kernel/component.hpp"

using namespace std;

const size_t INDENT_SIZE = 2;

vector3_t Entity::VECTOR_ZERO = vector3_t(0.0f, 0.0f, 0.0f);
vector3_t Entity::VECTOR_X_AXIS = vector3_t(1.0f, 0.0f, 0.0f);
vector3_t Entity::VECTOR_Y_AXIS = vector3_t(0.0f, 1.0f, 0.0f);
vector3_t Entity::VECTOR_Z_AXIS = vector3_t(0.0f, 0.0f, 1.0f);

Entity::Entity(const Entity* parent, const string& objectName):
    CommandObject(objectName),
    m_parent(*parent),
    m_children(),
    m_components(TOTAL_COMPONENTS_CONTAINER_SIZE, 0),
    m_positionRel(0.0f, 0.0f, 0.0f),
    m_positionAbs(0.0f, 0.0f, 0.0f),
    m_rotation(quaternion_t::getIdentity()),
    m_lastRotation(quaternion_t::getIdentity())
{
    registerAttribute("position", boost::bind(&Entity::setPosition, this, _1));
    registerCommand("move-xyz", boost::bind(&Entity::moveXYZ, this, _1));
    registerCommand("move-x", boost::bind(&Entity::moveX, this, _1));
    registerCommand("move-y", boost::bind(&Entity::moveY, this, _1));
    registerCommand("move-z", boost::bind(&Entity::moveZ, this, _1));
    registerCommand("move-xyz-parent", boost::bind(&Entity::moveXYZ_parent, this, _1));
    registerCommand("move-x-parent", boost::bind(&Entity::moveX_parent, this, _1));
    registerCommand("move-y-parent", boost::bind(&Entity::moveY_parent, this, _1));
    registerCommand("move-z-parent", boost::bind(&Entity::moveZ_parent, this, _1));
    registerCommand("move-xyz-global", boost::bind(&Entity::moveXYZ_global, this, _1));
    registerCommand("move-x-global", boost::bind(&Entity::moveX_global, this, _1));
    registerCommand("move-y-global", boost::bind(&Entity::moveY_global, this, _1));
    registerCommand("move-z-global", boost::bind(&Entity::moveZ_global, this, _1));
    registerCommand("yaw", boost::bind(&Entity::yaw, this, _1));
    registerCommand("pitch", boost::bind(&Entity::pitch, this, _1));
    registerCommand("roll", boost::bind(&Entity::roll, this, _1));
    registerCommand("yaw-parent", boost::bind(&Entity::yaw_parent, this, _1));
    registerCommand("pitch-parent", boost::bind(&Entity::pitch_parent, this, _1));
    registerCommand("roll-parent", boost::bind(&Entity::roll_parent, this, _1));
    registerCommand("yaw-global", boost::bind(&Entity::yaw_global, this, _1));
    registerCommand("pitch-global", boost::bind(&Entity::pitch_global, this, _1));
    registerCommand("roll-global", boost::bind(&Entity::roll_global, this, _1));
}

Entity::~Entity() {
    set<Entity*>::iterator it;
    for (it = m_children.begin(); it != m_children.end(); ++it)
        delete *it;
    for (size_t i = 0; i < m_components.size(); ++i)
        delete m_components[i];
}

void Entity::translate(const vector3_t& displacement, const transform_space_t relativeTo) {
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

void Entity::rotate(const quaternion_t& rotation, const transform_space_t relativeTo) {
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

void Entity::setDirection(const vector3_t& target) {
    if (target == VECTOR_ZERO)
        return;
    cerr << "Transform.setDirection(vector3) not implemented yet!" << endl;
}

vector3_t Entity::rotateVector(const vector3_t& v, const quaternion_t& rotation) {
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

void Entity::calcOpenGLMatrix(float* m) const {
    transform_t t(m_rotation, m_positionAbs);
    t.getOpenGLMatrix(m);
}

float Entity::calcYaw() const {
    return asin(-2.0 * (m_rotation.getX() * m_rotation.getZ() - m_rotation.getW() * m_rotation.getY()));
}

float Entity::calcPitch() const {
    return atan2(2.0 * (m_rotation.getY() * m_rotation.getZ() + m_rotation.getW() * m_rotation.getX()),
                 m_rotation.getW() * m_rotation.getW() -
                 m_rotation.getX() * m_rotation.getX() -
                 m_rotation.getY() * m_rotation.getY() +
                 m_rotation.getZ() * m_rotation.getZ());
}

float Entity::calcRoll() const {
    return atan2(2.0 * (m_rotation.getX() * m_rotation.getY() + m_rotation.getW() * m_rotation.getZ()),
                 m_rotation.getW() * m_rotation.getW() +
                 m_rotation.getX() * m_rotation.getX() -
                 m_rotation.getY() * m_rotation.getY() -
                 m_rotation.getZ() * m_rotation.getZ());
}

void Entity::applyTranslationToChildren() {
    set<Entity*>::iterator it, itend;
    itend = m_children.end();
    for (it = m_children.begin(); it != itend; ++it) {
        Entity& child = **it;
        child.setPositionRel(child.m_positionRel);
    }
}

void Entity::applyRotationToChildren() {
    quaternion_t relativeRotation = m_rotation * m_lastRotation.inverse();
    set<Entity*>::iterator it, itend;
    itend = m_children.end();
    for (it = m_children.begin(); it != itend; ++it) {
        Entity& child = **it;
        child.setPositionRel(rotateVector(child.m_positionRel, relativeRotation));
        child.setRotation(relativeRotation * child.m_rotation);
    }
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

void Entity::setPosition(const string& arg) {
    float x, y, z;
    stringstream ss(arg);
    ss >> x >> y >> z;
    setPositionAbs(x, y, z);
}

void Entity::moveXYZ(const std::string& arg) {
    float x, y, z;
    stringstream ss(arg);
    ss >> x >> y >> z;
    translate(x, y, z);
}

void Entity::moveX(const std::string& arg) {
    float dist;
    stringstream ss(arg);
    ss >> dist;
    translateX(dist * DeviceManager::getDeltaTime());
}

void Entity::moveY(const std::string& arg) {
    float dist;
    stringstream ss(arg);
    ss >> dist;
    translateY(dist * DeviceManager::getDeltaTime());
}

void Entity::moveZ(const std::string& arg) {
    float dist;
    stringstream ss(arg);
    ss >> dist;
    translateZ(dist * DeviceManager::getDeltaTime());
}

void Entity::moveXYZ_parent(const std::string& arg) {
    float x, y, z;
    stringstream ss(arg);
    ss >> x >> y >> z;
    translate(x, y, z, SPACE_PARENT);
}

void Entity::moveX_parent(const std::string& arg) {
    float dist;
    stringstream ss(arg);
    ss >> dist;
    translateX(dist * DeviceManager::getDeltaTime(), SPACE_PARENT);
}

void Entity::moveY_parent(const std::string& arg) {
    float dist;
    stringstream ss(arg);
    ss >> dist;
    translateY(dist * DeviceManager::getDeltaTime(), SPACE_PARENT);
}

void Entity::moveZ_parent(const std::string& arg) {
    float dist;
    stringstream ss(arg);
    ss >> dist;
    translateZ(dist * DeviceManager::getDeltaTime(), SPACE_PARENT);
}

void Entity::moveXYZ_global(const std::string& arg) {
    float x, y, z;
    stringstream ss(arg);
    ss >> x >> y >> z;
    translate(x, y, z, SPACE_GLOBAL);
}

void Entity::moveX_global(const std::string& arg) {
    float dist;
    stringstream ss(arg);
    ss >> dist;
    translateX(dist * DeviceManager::getDeltaTime(), SPACE_GLOBAL);
}

void Entity::moveY_global(const std::string& arg) {
    float dist;
    stringstream ss(arg);
    ss >> dist;
    translateY(dist * DeviceManager::getDeltaTime(), SPACE_GLOBAL);
}

void Entity::moveZ_global(const std::string& arg) {
    float dist;
    stringstream ss(arg);
    ss >> dist;
    translateZ(dist * DeviceManager::getDeltaTime(), SPACE_GLOBAL);
}

void Entity::yaw(const std::string& arg) {
    float radians;
    stringstream ss(arg);
    ss >> radians;
    yaw(radians);
}

void Entity::pitch(const std::string& arg) {
    float radians;
    stringstream ss(arg);
    ss >> radians;
    pitch(radians);
}

void Entity::roll(const std::string& arg) {
    float radians;
    stringstream ss(arg);
    ss >> radians;
    roll(radians);
}

void Entity::yaw_parent(const std::string& arg) {
    float radians;
    stringstream ss(arg);
    ss >> radians;
    yaw(radians, SPACE_PARENT);
}

void Entity::pitch_parent(const std::string& arg) {
    float radians;
    stringstream ss(arg);
    ss >> radians;
    pitch(radians, SPACE_PARENT);
}

void Entity::roll_parent(const std::string& arg) {
    float radians;
    stringstream ss(arg);
    ss >> radians;
    roll(radians, SPACE_PARENT);
}

void Entity::yaw_global(const std::string& arg) {
    float radians;
    stringstream ss(arg);
    ss >> radians;
    yaw(radians, SPACE_GLOBAL);
}

void Entity::pitch_global(const std::string& arg) {
    float radians;
    stringstream ss(arg);
    ss >> radians;
    pitch(radians, SPACE_GLOBAL);
}

void Entity::roll_global(const std::string& arg) {
    float radians;
    stringstream ss(arg);
    ss >> radians;
    roll(radians, SPACE_GLOBAL);
}

ostream& operator<<(ostream& out, const Entity& rhs) {
    out << "position(" << rhs.getPositionAbs().getX() << ", " <<
            rhs.getPositionAbs().getY() << ", " <<
            rhs.getPositionAbs().getZ() << ")" << endl;

    out << "rotation(" << rhs.getRotation().getW() << ", " <<
            rhs.getRotation().getX() << ", " <<
            rhs.getRotation().getY() << ", " <<
            rhs.getRotation().getZ() << ")" << endl;

//     out << "scale(" << rhs.getScale().getX() << ", " <<
//             rhs.getScale().getY() << ", " <<
//             rhs.getScale().getZ() << ")" << endl;

    for (size_t i = 0; i < rhs.m_components.size(); ++i) {
        if (rhs.m_components[i] != 0)
            out << "    " << *rhs.m_components[i] << endl;
    }
    return out;
}
