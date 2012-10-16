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


#ifndef PHYSICSWORLD_HPP
#define PHYSICSWORLD_HPP

#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

const double FIXED_TIMESTEP = 1.0 / 60.0;

class PhysicsManager;
class RigidBody;
class Entity;

// Bullet classes needed for world simulation
class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btCollisionShape;
class btRigidBody;

class PhysicsWorld {
public:
    friend class PhysicsManager;
    friend class RigidBody;

    void setMinExpectedFramerate(const double minExpectedFramerate);
    void setGravity(const double x, const double y, const double z);
    void stepSimulation(const double currentTimeSeconds);

private:
    PhysicsWorld();
    PhysicsWorld(const PhysicsWorld& rhs);

    PhysicsWorld& operator=(const PhysicsWorld& rhs);

    void initialize();
    void deinitialize();
    void updateRigidBodies();

    int m_maxSubsteps;
    double m_lastTime;
    typedef boost::unordered_map<std::string, btCollisionShape*> collision_shapes_map_t;
    typedef boost::unordered_map<Entity*, btRigidBody*> rigid_bodies_map_t;
    btBroadphaseInterface* m_broadphase;
    btDefaultCollisionConfiguration* m_collisionConfiguration;
    btCollisionDispatcher* m_collisionDispatcher;
    btSequentialImpulseConstraintSolver* m_solver;
    btDiscreteDynamicsWorld* m_dynamicsWorld;
    collision_shapes_map_t m_collisionShapes;
    rigid_bodies_map_t m_rigidBodies;
};

inline void PhysicsWorld::setMinExpectedFramerate(const double minExpectedFramerate) {
    double minTimestep = 1.0 / minExpectedFramerate;
    m_maxSubsteps = minTimestep / FIXED_TIMESTEP + 2;
}

#endif // PHYSICSWORLD_HPP
