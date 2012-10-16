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


#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include "engine/kernel/component.hpp"
#include "engine/kernel/vector3.hpp"

class btRigidBody;
class btCollisionShape;
class btDefaultMotionState;
class btVector3;

class RigidBody: public Component {
public:
    RigidBody(Entity* const entity);

    double getMass() const;
    double getLinearDamping() const;
    double getAngularDamping() const;
    double getFriction() const;
    double getRollingFriction() const;
    double getRestitution() const;
    double getLinearSleepingThreshold() const;
    double getAngularSleepingThreshold() const;
    const Vector3& getLinearFactor() const;
    const Vector3& getLinearVelocity() const;
    const Vector3& getAngularFactor() const;
    const Vector3& getAngularVelocity() const;
    const Vector3& getGravity() const;

    void init(const double mass = 0.0,
              const double friction = 0.5,
              const double rollingFriction = 0.1,
              const double linearDamping = 0.0,
              const double angularDamping = 0.0,
              const double linearSleepingThreshold = 0.8,
              const double angularSleepingThreshold = 1.0,
              const double restitution = 0.0);
    void setDamping(const double linear, const double angular);
    void setFriction(const double friction);
    void setRollingFriction(const double rollingFriction);
    void setRestitution(const double restitution);
    void setSleepingThresholds(const double linear, const double angular);
    void setLinearFactor(const Vector3& linearFactor);
    void setLinearVelocity(const Vector3& linearVelocity);
    void setAngularFactor(const Vector3& angularFactor);
    void setAngularVelocity(const Vector3& angularVelocity);
    void setGravity(const Vector3& gravity);

    void addBox(const double lengthX, const double lengthY, const double lengthZ);
    void addSphere(const double radius);

private:
    btRigidBody* m_rigidBody;
    double m_mass;
    double m_linearDamping;
    double m_angularDamping;
    double m_friction;
    double m_rollingFriction;
    double m_restitution;
    double m_linearSleepingThreshold;
    double m_angularSleepingThreshold;
    Vector3 m_linearFactor;
    Vector3 m_linearVelocity;
    Vector3 m_angularFactor;
    Vector3 m_angularVelocity;
    Vector3 m_gravity;

    RigidBody(const RigidBody& rhs);
    RigidBody& operator=(const RigidBody& rhs);

    void addRigidBody(btCollisionShape* shape);
};



inline double RigidBody::getMass() const {
    return m_mass;
}

inline double RigidBody::getLinearDamping() const {
    return m_linearDamping;
}

inline double RigidBody::getAngularDamping() const {
    return m_angularDamping;
}

inline double RigidBody::getFriction() const {
    return m_friction;
}

inline double RigidBody::getRollingFriction() const {
    return m_rollingFriction;
}

inline double RigidBody::getRestitution() const {
    return m_restitution;
}

inline double RigidBody::getLinearSleepingThreshold() const {
    return m_linearSleepingThreshold;
}

inline double RigidBody::getAngularSleepingThreshold() const {
    return m_angularSleepingThreshold;
}

inline const Vector3& RigidBody::getLinearFactor() const {
    return m_linearFactor;
}

inline const Vector3& RigidBody::getLinearVelocity() const {
    return m_linearVelocity;
}

inline const Vector3& RigidBody::getAngularFactor() const {
    return m_angularFactor;
}

inline const Vector3& RigidBody::getAngularVelocity() const {
    return m_angularVelocity;
}

inline const Vector3& RigidBody::getGravity() const {
    return m_gravity;
}

#endif // RIGIDBODY_HPP
