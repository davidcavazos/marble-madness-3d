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
btVector3 v3(const Vector3& v);



RigidBody::RigidBody(Entity* const entity):
    Component(COMPONENT_PHYSICS, entity),
    m_rigidBody(0),
    m_mass(0.0),
    m_linearDamping(0.0),
    m_angularDamping(0.0),
    m_friction(0.5),
    m_rollingFriction(0.0),
    m_restitution(0.0),
    m_linearSleepingThreshold(0.8),
    m_angularSleepingThreshold(1.0),
    m_linearFactor(VECTOR3_ZERO),
    m_linearVelocity(VECTOR3_ZERO),
    m_angularFactor(VECTOR3_ZERO),
    m_angularVelocity(VECTOR3_ZERO),
    m_gravity(VECTOR3_ZERO)
{}

RigidBody::RigidBody(const RigidBody& rhs):
    Component(COMPONENT_PHYSICS, &rhs.m_entity),
    m_rigidBody(rhs.m_rigidBody),
    m_mass(rhs.m_mass),
    m_linearDamping(rhs.m_linearDamping),
    m_angularDamping(rhs.m_angularDamping),
    m_friction(rhs.m_friction),
    m_rollingFriction(rhs.m_rollingFriction),
    m_restitution(rhs.m_restitution),
    m_linearSleepingThreshold(rhs.m_linearSleepingThreshold),
    m_angularSleepingThreshold(rhs.m_angularSleepingThreshold),
    m_linearFactor(rhs.m_linearFactor),
    m_linearVelocity(rhs.m_linearVelocity),
    m_angularFactor(rhs.m_angularFactor),
    m_angularVelocity(rhs.m_angularVelocity),
    m_gravity(rhs.m_gravity)
{}

RigidBody& RigidBody::operator=(const RigidBody& rhs) {
    if (this == &rhs)
        return *this;
    m_rigidBody = rhs.m_rigidBody;
    m_mass = rhs.m_mass;
    m_linearDamping = rhs.m_linearDamping;
    m_angularDamping = rhs.m_angularDamping;
    m_friction = rhs.m_friction;
    m_rollingFriction = rhs.m_rollingFriction;
    m_restitution = rhs.m_restitution;
    m_linearSleepingThreshold = rhs.m_linearSleepingThreshold;
    m_angularSleepingThreshold = rhs.m_angularSleepingThreshold;
    m_linearFactor = rhs.m_linearFactor;
    m_linearVelocity = rhs.m_linearVelocity;
    m_angularFactor = rhs.m_angularFactor;
    m_angularVelocity = rhs.m_angularVelocity;
    m_gravity = rhs.m_gravity;
    return *this;
}

void RigidBody::init(const double mass,
                     const double friction,
                     const double rollingFriction,
                     const double linearDamping,
                     const double angularDamping,
                     const double linearSleepingThreshold,
                     const double angularSleepingThreshold,
                     const double restitution) {
    m_mass = mass;
    m_friction = friction;
    m_rollingFriction = rollingFriction;
    m_linearDamping = linearDamping;
    m_angularDamping = angularDamping;
    m_linearSleepingThreshold = linearSleepingThreshold;
    m_angularSleepingThreshold = angularSleepingThreshold;
    m_restitution = restitution;
}

void RigidBody::setDamping(const double linear, const double angular) {
    m_linearDamping = linear;
    m_angularDamping = angular;
    m_rigidBody->setDamping(m_linearDamping, m_angularDamping);
}

void RigidBody::setFriction(const double friction) {
    m_friction = friction;
    m_rigidBody->setFriction(m_friction);
}

void RigidBody::setRollingFriction(const double rollingFriction) {
    m_rollingFriction = rollingFriction;
    m_rigidBody->setRollingFriction(m_rollingFriction);
}

void RigidBody::setRestitution(const double restitution) {
    m_restitution = restitution;
    m_rigidBody->setRestitution(m_restitution);
}

void RigidBody::setSleepingThresholds(const double linear, const double angular) {
    m_linearSleepingThreshold = linear;
    m_angularSleepingThreshold = angular;
    m_rigidBody->setSleepingThresholds(m_linearSleepingThreshold, m_angularSleepingThreshold);
}

void RigidBody::setLinearFactor(const Vector3& linearFactor) {
    m_linearFactor = linearFactor;
    m_rigidBody->setLinearFactor(v3(linearFactor));
}

void RigidBody::setLinearVelocity(const Vector3& linearVelocity) {
    m_linearVelocity = linearVelocity;
    m_rigidBody->setLinearVelocity(v3(m_linearVelocity));
}

void RigidBody::setAngularFactor(const Vector3& angularFactor) {
    m_angularFactor = angularFactor;
    m_rigidBody->setAngularFactor(v3(m_angularFactor));
}

void RigidBody::setAngularVelocity(const Vector3& angularVelocity) {
    m_angularVelocity = angularVelocity;
    m_rigidBody->setAngularVelocity(v3(m_angularVelocity));
}

void RigidBody::setGravity(const Vector3& gravity) {
    m_gravity = gravity;
    m_rigidBody->setGravity(v3(m_gravity));
}

void RigidBody::addBox(const double lengthX, const double lengthY, const double lengthZ) {
    stringstream ss;
    ss << COLLISION_SHAPE_BOX << "_" << lengthX << "_" << lengthY << "_" << lengthZ;
    string shapeId = ss.str();

    PhysicsWorld::collision_shapes_map_t& collisionShapes = PhysicsManager::getPhysicsWorld().m_collisionShapes;
    PhysicsWorld::collision_shapes_map_t::const_iterator it;

    btCollisionShape* shape;
    it = collisionShapes.find(shapeId);
    if (it == collisionShapes.end()) {
        shape = new btBoxShape(btVector3(lengthX * 0.5, lengthY * 0.5, lengthZ * 0.5));
        collisionShapes.insert(std::pair<string, btCollisionShape*>(shapeId, shape));
    }
    else
        shape = it->second;
    addRigidBody(shape);
}

void RigidBody::addSphere(const double radius) {
    stringstream ss;
    ss << COLLISION_SHAPE_BOX << "_" << radius;
    string shapeId = ss.str();

    PhysicsWorld::collision_shapes_map_t& collisionShapes = PhysicsManager::getPhysicsWorld().m_collisionShapes;
    PhysicsWorld::collision_shapes_map_t::const_iterator it;

    btCollisionShape* shape;
    it = collisionShapes.find(shapeId);
    if (it == collisionShapes.end()) {
        shape = new btSphereShape(radius);
        collisionShapes.insert(std::pair<string, btCollisionShape*>(shapeId, shape));
    }
    else
        shape = it->second;
    addRigidBody(shape);
}

void RigidBody::addRigidBody(btCollisionShape* shape) {
    btDefaultMotionState* motion = getMotionState(m_entity);
    btVector3 inertia = getFallInertia(m_mass, shape);
    btRigidBody::btRigidBodyConstructionInfo constructionInfo(m_mass, motion, shape, inertia);
    constructionInfo.m_friction = m_friction;
    constructionInfo.m_rollingFriction = m_rollingFriction;
    constructionInfo.m_linearDamping = m_linearDamping;
    constructionInfo.m_angularDamping = m_angularDamping;
    constructionInfo.m_linearSleepingThreshold = m_linearSleepingThreshold;
    constructionInfo.m_angularSleepingThreshold = m_angularSleepingThreshold;
    constructionInfo.m_restitution = m_restitution;
    m_rigidBody = new btRigidBody(constructionInfo);
    PhysicsManager::getPhysicsWorld().m_rigidBodies.insert(
        std::pair<Entity*, btRigidBody*>(&m_entity, m_rigidBody)
    );
    PhysicsManager::getPhysicsWorld().m_dynamicsWorld->addRigidBody(m_rigidBody);
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
    btVector3 inertia;
    shape->calculateLocalInertia(mass, inertia);
    return inertia;
}

btVector3 v3(const Vector3& v) {
    return btVector3(v.getX(), v.getY(), v.getZ());
}
