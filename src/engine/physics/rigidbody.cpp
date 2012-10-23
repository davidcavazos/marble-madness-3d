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
#include <iostream>
#include <sstream>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionShapes/btShapeHull.h>
#include "engine/kernel/entity.hpp"
#include "engine/physics/physicsmanager.hpp"
#include "engine/physics/physicsworld.hpp"
#include <engine/resources/model.hpp>
#include <engine/resources/resourcemanager.hpp>
#include <engine/resources/resources.hpp>

using namespace std;

const string COLLISION_SHAPE_BOX = "box";
const string COLLISION_SHAPE_SPHERE = "sphere";
const string COLLISION_SHAPE_CYLINDER = "cylinder";
const string COLLISION_SHAPE_CAPSULE = "capsule";
const string COLLISION_SHAPE_CONE = "cone";
const string COLLISION_SHAPE_CONVEX = "convex";
const string COLLISION_SHAPE_CONCAVE = "concave";

btDefaultMotionState* getMotionState(const Entity& entity);
btVector3 v3(const Vector3& v);
btQuaternion quat(const Quaternion& q);



RigidBody::RigidBody(Entity* const entity):
    Component(COMPONENT_PHYSICS, entity),
    m_rigidBody(0),
    m_position(VECTOR3_ZERO),
    m_orientation(QUATERNION_IDENTITY),
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
{
    m_entity.registerAttribute("mass", boost::bind(&RigidBody::cmdMass, this, _1));
    m_entity.registerAttribute("damping", boost::bind(&RigidBody::cmdDamping, this, _1));
    m_entity.registerAttribute("friction", boost::bind(&RigidBody::cmdFriction, this, _1));
    m_entity.registerAttribute("rolling-friction", boost::bind(&RigidBody::cmdRollingFriction, this, _1));
    m_entity.registerAttribute("restitution", boost::bind(&RigidBody::cmdRestitution, this, _1));
    m_entity.registerAttribute("sleeping-thresholds", boost::bind(&RigidBody::cmdSleepingThresholds, this, _1));
    m_entity.registerAttribute("linear-factor", boost::bind(&RigidBody::cmdLinearFactor, this, _1));
    m_entity.registerAttribute("linear-velocity", boost::bind(&RigidBody::cmdLinearVelocity, this, _1));
    m_entity.registerAttribute("angular-factor", boost::bind(&RigidBody::cmdAngularFactor, this, _1));
    m_entity.registerAttribute("angular-velocity", boost::bind(&RigidBody::cmdAngularVelocity, this, _1));
    m_entity.registerAttribute("gravity", boost::bind(&RigidBody::cmdGravity, this, _1));
}

RigidBody::RigidBody(const RigidBody& rhs):
    Component(COMPONENT_PHYSICS, &rhs.m_entity),
    m_rigidBody(rhs.m_rigidBody),
    m_position(rhs.m_position),
    m_orientation(rhs.m_orientation),
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
    m_position = rhs.m_position;
    m_orientation = rhs.m_orientation;
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

void RigidBody::setTransform(const Vector3& position, const Quaternion& orientation) {
    m_position = position;
    m_orientation = orientation;
    m_rigidBody->setWorldTransform(btTransform(quat(m_orientation), v3(m_position)));
}

void RigidBody::setMass(const double mass) {
    m_mass = mass;
    btVector3 inertia;
    m_rigidBody->getCollisionShape()->calculateLocalInertia(m_mass, inertia);
    m_rigidBody->setMassProps(mass, inertia);
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

void RigidBody::addCylinder(const double radius, const double height) {
    stringstream ss;
    ss << COLLISION_SHAPE_CYLINDER << "_" << radius << "_" << height;
    string shapeId = ss.str();

    PhysicsWorld::collision_shapes_map_t& collisionShapes = PhysicsManager::getPhysicsWorld().m_collisionShapes;
    PhysicsWorld::collision_shapes_map_t::const_iterator it;

    btCollisionShape* shape;
    it = collisionShapes.find(shapeId);
    if (it == collisionShapes.end()) {
        shape = new btCylinderShape(btVector3(radius, height, radius));
        collisionShapes.insert(std::pair<string, btCollisionShape*>(shapeId, shape));
    }
    else
        shape = it->second;
    addRigidBody(shape);
}

void RigidBody::addCapsule(const double radius, const double height) {
    stringstream ss;
    ss << COLLISION_SHAPE_CAPSULE << "_" << radius << "_" << height;
    string shapeId = ss.str();

    PhysicsWorld::collision_shapes_map_t& collisionShapes = PhysicsManager::getPhysicsWorld().m_collisionShapes;
    PhysicsWorld::collision_shapes_map_t::const_iterator it;

    btCollisionShape* shape;
    it = collisionShapes.find(shapeId);
    if (it == collisionShapes.end()) {
        shape = new btCapsuleShape(radius, height);
        collisionShapes.insert(std::pair<string, btCollisionShape*>(shapeId, shape));
    }
    else
        shape = it->second;
    addRigidBody(shape);
}

void RigidBody::addCone(const double radius, const double height) {
    stringstream ss;
    ss << COLLISION_SHAPE_CONE << "_" << radius << "_" << height;
    string shapeId = ss.str();

    PhysicsWorld::collision_shapes_map_t& collisionShapes = PhysicsManager::getPhysicsWorld().m_collisionShapes;
    PhysicsWorld::collision_shapes_map_t::const_iterator it;

    btCollisionShape* shape;
    it = collisionShapes.find(shapeId);
    if (it == collisionShapes.end()) {
        shape = new btConeShape(radius, height);
        collisionShapes.insert(std::pair<string, btCollisionShape*>(shapeId, shape));
    }
    else
        shape = it->second;
    addRigidBody(shape);
}

void RigidBody::addConvexHull(const string& fileName) {
    stringstream ss;
    ss << COLLISION_SHAPE_CONVEX << "_" << fileName;
    string shapeId = ss.str();

    PhysicsWorld::collision_shapes_map_t& collisionShapes = PhysicsManager::getPhysicsWorld().m_collisionShapes;
    PhysicsWorld::collision_shapes_map_t::const_iterator it;

    btCollisionShape* shape;
    it = collisionShapes.find(shapeId);
    if (it == collisionShapes.end()) {
        // build original mesh from file
        Model* model = ResourceManager::getResources().generateModelFromFile(fileName);
        vector<float> points;
        for (size_t n = 0; n < model->getTotalMeshes(); ++n) {
            points.reserve(points.size() + model->mesh(n).getTotalVertices());
            for (size_t i = 0; i < model->mesh(n).getTotalVertices(); ++i)
                points.push_back(model->mesh(n).getVertices()[i]);
        }
        btConvexShape* originalConvexShape = new btConvexHullShape(&points[0], points.size(), sizeof(float) * 3);
        points.clear();

        // convert to low polygon hull
        btShapeHull* hull = new btShapeHull(originalConvexShape);
        btScalar margin = originalConvexShape->getMargin();
        hull->buildHull(margin);

        shape = new btConvexHullShape(&hull->getVertexPointer()->getX(), hull->numVertices());
        collisionShapes.insert(std::pair<string, btCollisionShape*>(shapeId, shape));

        delete originalConvexShape;
        delete hull;
    }
    else
        shape = it->second;
    addRigidBody(shape);
}

void RigidBody::addConcaveHull(const string& fileName) {
    stringstream ss;
    ss << COLLISION_SHAPE_CONCAVE << "_" << fileName;
    string shapeId = ss.str();

    PhysicsWorld::collision_shapes_map_t& collisionShapes = PhysicsManager::getPhysicsWorld().m_collisionShapes;
    PhysicsWorld::collision_shapes_map_t::const_iterator it;

    btCollisionShape* shape;
    it = collisionShapes.find(shapeId);
    if (it == collisionShapes.end()) {
        // build mesh from file
        Model* model = ResourceManager::getResources().generateModelFromFile(fileName);
        btTriangleIndexVertexArray* triangles = new btTriangleIndexVertexArray();
        for (size_t n = 0; n < model->getTotalMeshes(); ++n) {
            btIndexedMesh indexedMesh;
            indexedMesh.m_numTriangles = model->mesh(n).getTotalIndices() / 3;
            indexedMesh.m_triangleIndexBase = (const unsigned char*)model->mesh(n).getIndicesPtr();
            indexedMesh.m_triangleIndexStride = sizeof(unsigned int);
            indexedMesh.m_numVertices = model->mesh(n).getTotalVertices();
            indexedMesh.m_vertexBase = (const unsigned char*)model->mesh(n).getVerticesPtr();
            indexedMesh.m_vertexStride = sizeof(float);
            triangles->addIndexedMesh(indexedMesh);
        }

        shape = new btBvhTriangleMeshShape(triangles, true, true);
        collisionShapes.insert(std::pair<string, btCollisionShape*>(shapeId, shape));

        delete triangles;
    }
    else
        shape = it->second;
    addRigidBody(shape);
}

void RigidBody::addRigidBody(btCollisionShape* shape) {
    btVector3 inertia;
    shape->calculateLocalInertia(m_mass, inertia);
    btDefaultMotionState* motion = getMotionState(m_entity);
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


void RigidBody::cmdMass(const string& arg) {
    double mass;
    stringstream ss(arg);
    ss >> mass;
    setMass(mass);
}

void RigidBody::cmdDamping(const string& arg) {
    double linear, angular;
    stringstream ss(arg);
    ss >> linear >> angular;
    setDamping(linear, angular);
}

void RigidBody::cmdFriction(const string& arg) {
    double friction;
    stringstream ss(arg);
    ss >> friction;
    setFriction(friction);
}

void RigidBody::cmdRollingFriction(const string& arg) {
    double rollingFriction;
    stringstream ss(arg);
    ss >> rollingFriction;
    setRollingFriction(rollingFriction);
}

void RigidBody::cmdRestitution(const string& arg) {
    double restitution;
    stringstream ss(arg);
    ss >> restitution;
    setRestitution(restitution);
}

void RigidBody::cmdSleepingThresholds(const string& arg) {
    double linear, angular;
    stringstream ss(arg);
    ss >> linear >> angular;
    setSleepingThresholds(linear, angular);
}

void RigidBody::cmdLinearFactor(const string& arg) {
    double x, y, z;
    stringstream ss(arg);
    ss >> x >> y >> z;
    setLinearFactor(Vector3(x, y, z));
}

void RigidBody::cmdLinearVelocity(const string& arg) {
    double x, y, z;
    stringstream ss(arg);
    ss >> x >> y >> z;
    setLinearVelocity(Vector3(x, y, z));
}

void RigidBody::cmdAngularFactor(const string& arg) {
    double x, y, z;
    stringstream ss(arg);
    ss >> x >> y >> z;
    setAngularFactor(Vector3(x, y, z));
}

void RigidBody::cmdAngularVelocity(const string& arg) {
    double x, y, z;
    stringstream ss(arg);
    ss >> x >> y >> z;
    setAngularVelocity(Vector3(x, y, z));
}

void RigidBody::cmdGravity(const string& arg) {
    double x, y, z;
    stringstream ss(arg);
    ss >> x >> y >> z;
    setGravity(Vector3(x, y, z));
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

btVector3 v3(const Vector3& v) {
    return btVector3(v.getX(), v.getY(), v.getZ());
}

btQuaternion quat(const Quaternion& q) {
    return btQuaternion(q.getX(), q.getY(), q.getZ(), q.getW());
}
