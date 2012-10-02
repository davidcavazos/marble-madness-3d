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

Entity::Entity(const Entity* parent, const string& objectName):
    CommandObject(objectName),
    m_parent(*parent),
    m_children(),
    m_components(TOTAL_COMPONENTS_CONTAINER_SIZE, 0),
    m_transform()
{
    registerAttribute("position", boost::bind(&Entity::setPosition, this, _1));
    registerCommand("move-xyz", boost::bind(&Entity::moveXYZ, this, _1));
    registerCommand("move-x", boost::bind(&Entity::moveX, this, _1));
    registerCommand("move-y", boost::bind(&Entity::moveY, this, _1));
    registerCommand("move-z", boost::bind(&Entity::moveZ, this, _1));
    registerCommand("rotate-ypr", boost::bind(&Entity::rotateYPR, this, _1));
    registerCommand("pitch", boost::bind(&Entity::pitch, this, _1));
    registerCommand("yaw", boost::bind(&Entity::yaw, this, _1));
    registerCommand("roll", boost::bind(&Entity::roll, this, _1));
}

Entity::~Entity() {
    set<Entity*>::iterator it;
    for (it = m_children.begin(); it != m_children.end(); ++it)
        delete *it;
    for (size_t i = 0; i < m_components.size(); ++i)
        delete m_components[i];
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
    m_transform.setPosition(x, y, z);
}

void Entity::moveXYZ(const std::string& arg) {
    float x, y, z;
    stringstream ss(arg);
    ss >> x >> y >> z;
    m_transform.translate(x, y, z);
}

void Entity::moveX(const std::string& arg) {
    float dist;
    stringstream ss(arg);
    ss >> dist;
    m_transform.translateX(dist * DeviceManager::getDeltaTime());
}

void Entity::moveY(const std::string& arg) {
    float dist;
    stringstream ss(arg);
    ss >> dist;
    m_transform.translateY(dist * DeviceManager::getDeltaTime());
}

void Entity::moveZ(const std::string& arg) {
    float dist;
    stringstream ss(arg);
    ss >> dist;
    m_transform.translateZ(dist * DeviceManager::getDeltaTime());
}

void Entity::rotateYPR(const std::string& arg) {
    float y, p, r;
    stringstream ss(arg);
    ss >> y >> p >> r;
    m_transform.rotate(y, p, r);
}

void Entity::yaw(const std::string& arg) {
    float radians;
    stringstream ss(arg);
    ss >> radians;
    m_transform.yaw(radians);
}

void Entity::pitch(const std::string& arg) {
    float radians;
    stringstream ss(arg);
    ss >> radians;
    m_transform.pitch(radians);
}

void Entity::roll(const std::string& arg) {
    float radians;
    stringstream ss(arg);
    ss >> radians;
    m_transform.roll(radians);
}

ostream& operator<<(ostream& out, const Entity& rhs) {
    out << "position(" << rhs.m_transform.getPosition().getX() << ", " <<
            rhs.m_transform.getPosition().getY() << ", " <<
            rhs.m_transform.getPosition().getZ() << ")" << endl;

    out << "rotation(" << rhs.m_transform.getRotation().getW() << ", " <<
            rhs.m_transform.getRotation().getX() << ", " <<
            rhs.m_transform.getRotation().getY() << ", " <<
            rhs.m_transform.getRotation().getZ() << ")" << endl;

    out << "scale(" << rhs.m_transform.getScale().getX() << ", " <<
            rhs.m_transform.getScale().getY() << ", " <<
            rhs.m_transform.getScale().getZ() << ")" << endl;

    for (size_t i = 0; i < rhs.m_components.size(); ++i) {
        if (rhs.m_components[i] != 0)
            out << "    " << *rhs.m_components[i] << endl;
    }
    return out;
}
