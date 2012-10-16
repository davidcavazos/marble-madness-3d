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


#include "engine/physics/rigidbody.hpp"

#include <string>
#include <sstream>
#include <bullet/btBulletDynamicsCommon.h>
#include "engine/kernel/entity.hpp"
#include "engine/physics/physicsmanager.hpp"
#include "engine/physics/physicsworld.hpp"

using namespace std;

const string COLLISION_SHAPE_BOX = "box";
const string COLLISION_SHAPE_SPHERE = "sph";

btDefaultMotionState* getMotionState(const Entity& entity);
btVector3 getFallInertia(const double mass, btCollisionShape* shape);



RigidBody::RigidBody(Entity* const entity):
    Component(COMPONENT_PHYSICS, entity)
{}

void RigidBody::generateCollisionBox(const double mass, const double lengthX, const double lengthY, const double lengthZ) const {
    stringstream ss;
    ss << COLLISION_SHAPE_BOX << "_" << lengthX << "_" << lengthY << "_" << lengthZ;
    string shapeId = ss.str();

    btCollisionShape* shape;
    if (!shapeIsFound(shapeId, shape))
        shape = new btBoxShape(btVector3(lengthX * 0.5, lengthY * 0.5, lengthZ * 0.5));
    addRigidBody(mass, shape);
}

void RigidBody::generateCollisionSphere(const double mass, const double radius) const {
    stringstream ss;
    ss << COLLISION_SHAPE_BOX << "_" << radius;
    string shapeId = ss.str();

    btCollisionShape* shape;
    if (!shapeIsFound(shapeId, shape))
        shape = new btSphereShape(radius);
    addRigidBody(mass, shape);
}

bool RigidBody::shapeIsFound(const string& shapeId, btCollisionShape*& shape) const {
    PhysicsWorld::collision_shapes_map_t& collisionShapes = PhysicsManager::getPhysicsWorld().m_collisionShapes;
    PhysicsWorld::collision_shapes_map_t::const_iterator it;

    it = collisionShapes.find(shapeId);
    if (it == collisionShapes.end()) {
        collisionShapes.insert(std::pair<string, btCollisionShape*>(shapeId, shape));
        return false;
    }
    shape = it->second;
    return true;
}

void RigidBody::addRigidBody(const double mass, btCollisionShape* shape) const {
    btDefaultMotionState* motion = getMotionState(m_entity);
    btVector3 inertia = getFallInertia(mass, shape);
    btRigidBody* rigidBody = new btRigidBody(mass, motion, shape, inertia);
    PhysicsManager::getPhysicsWorld().m_rigidBodies.insert(
        std::pair<Entity*, btRigidBody*>(&m_entity, rigidBody)
    );
    PhysicsManager::getPhysicsWorld().m_dynamicsWorld->addRigidBody(rigidBody);
}



btDefaultMotionState* getMotionState(const Entity& entity) {
    const Quaternion& rot = entity.getOrientationAbs();
    const Vector3& pos = entity.getPositionAbs();
    btDefaultMotionState* motion =
    new btDefaultMotionState(btTransform(
        btQuaternion(rot.getX(), rot.getY(), rot.getZ(), rot.getW()),
        btVector3(pos.getX(), pos.getY(), pos.getZ()))
    );
    return motion;
}

btVector3 getFallInertia(const double mass, btCollisionShape* shape) {
    btVector3 inertia(0, 0, 0);
    if (mass > 0.0)
        shape->calculateLocalInertia(mass, inertia);
    return inertia;
}
