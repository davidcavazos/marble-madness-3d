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


#include "engine/physics/physicsworld.hpp"

#include <iostream>
#include <bullet/btBulletDynamicsCommon.h>
#include "engine/kernel/entity.hpp"

using namespace std;

const double DEFAULT_MIN_EXPECTED_FRAMERATE = 10.0;

void PhysicsWorld::setGravity(const double x, const double y, const double z) {
    m_dynamicsWorld->setGravity(btVector3(static_cast<btScalar>(x),
                                          static_cast<btScalar>(y),
                                          static_cast<btScalar>(z)));
}

void PhysicsWorld::stepSimulation(const double currentTimeSeconds) {
    m_dynamicsWorld->stepSimulation(currentTimeSeconds - m_lastTime, m_maxSubsteps, FIXED_TIMESTEP);
    m_lastTime = currentTimeSeconds;

    updateRigidBodies();
}

PhysicsWorld::PhysicsWorld():
    m_maxSubsteps(1),
    m_lastTime(0.0),
    m_broadphase(0),
    m_collisionConfiguration(0),
    m_collisionDispatcher(0),
    m_solver(0),
    m_dynamicsWorld(0),
    m_collisionShapes(),
    m_rigidBodies()
{
    cout << "Physics simulations done with Bullet Physics" << endl;
    setMinExpectedFramerate(DEFAULT_MIN_EXPECTED_FRAMERATE);
}

PhysicsWorld::PhysicsWorld(const PhysicsWorld& rhs):
    m_maxSubsteps(rhs.m_maxSubsteps),
    m_lastTime(rhs.m_lastTime),
    m_broadphase(rhs.m_broadphase),
    m_collisionConfiguration(rhs.m_collisionConfiguration),
    m_collisionDispatcher(rhs.m_collisionDispatcher),
    m_solver(rhs.m_solver),
    m_dynamicsWorld(rhs.m_dynamicsWorld),
    m_collisionShapes(rhs.m_collisionShapes),
    m_rigidBodies(rhs.m_rigidBodies)
{}

PhysicsWorld& PhysicsWorld::operator=(const PhysicsWorld& rhs) {
    if (this == &rhs)
        return *this;
    m_lastTime = rhs.m_lastTime;
    m_broadphase = rhs.m_broadphase;
    m_collisionConfiguration = rhs.m_collisionConfiguration;
    m_collisionDispatcher = rhs.m_collisionDispatcher;
    m_solver = rhs.m_solver;
    m_dynamicsWorld = rhs.m_dynamicsWorld;
    m_collisionShapes = rhs.m_collisionShapes;
    m_rigidBodies = rhs.m_rigidBodies;
    return *this;
}

void PhysicsWorld::initialize() {
    cout << "Creating dynamics world" << endl;
    m_broadphase = new btDbvtBroadphase;
    m_collisionConfiguration = new btDefaultCollisionConfiguration;
    m_collisionDispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_solver = new btSequentialImpulseConstraintSolver;
    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_collisionDispatcher, m_broadphase, m_solver, m_collisionConfiguration);

    cout << "Setting default gravity as -9.8 in the y-axis" << endl;
    setGravity(0.0, -9.8, 0.0);
}

void PhysicsWorld::deinitialize() {
    cout << "Destroying dynamics world" << endl;

    rigid_bodies_map_t::iterator itRig;
    for (itRig = m_rigidBodies.begin(); itRig != m_rigidBodies.end(); ++itRig) {
        m_dynamicsWorld->removeRigidBody(itRig->second);
        delete itRig->second->getMotionState();
        delete itRig->second;
    }

    collision_shapes_map_t::iterator itCol;
    for (itCol = m_collisionShapes.begin(); itCol != m_collisionShapes.end(); ++itCol)
        delete itCol->second;

    delete m_dynamicsWorld;
    delete m_solver;
    delete m_collisionDispatcher;
    delete m_collisionConfiguration;
    delete m_broadphase;
}

void PhysicsWorld::updateRigidBodies() {
    rigid_bodies_map_t::const_iterator it;
    btTransform trans;
    btQuaternion rot;
    for (it = m_rigidBodies.begin(); it != m_rigidBodies.end(); ++it) {
        btRigidBody* rigidBody = it->second;
        Entity* entity = it->first;

        rigidBody->getMotionState()->getWorldTransform(trans);
        btVector3& pos = trans.getOrigin();
        rot = trans.getRotation();

        entity->setPositionAbs(pos.getX(), pos.getY(), pos.getZ());
        entity->setOrientationAbs(rot.getW(), rot.getX(), rot.getY(), rot.getZ());
    }
}
