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

// typedef enum {
//     PHYSICS_RIGIDBODY,
//     PHYSICS_SOFTBODY
// } physics_t;

class btCollisionShape;
class btDefaultMotionState;
class btVector3;

class RigidBody: public Component {
public:
    RigidBody(Entity* const entity);

    void generateCollisionBox(const double mass, const double lengthX, const double lengthY, const double lengthZ) const;
    void generateCollisionSphere(const double mass, const double radius) const;

private:
    bool shapeIsFound(const std::string& shapeId, btCollisionShape*& shape) const;
    void addRigidBody(const double mass, btCollisionShape* shape) const;
};

#endif // RIGIDBODY_HPP
