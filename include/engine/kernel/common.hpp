/*
 *    <one line to give the library's name and an idea of what it does.>
 *    Copyright (C) 2012  David Cavazos <davido262@gmail.com>
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 2.1 of the License, or (at your option) any later version.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with this library; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */


#ifndef COMMON_HPP
#define COMMON_HPP

#include <boost/math/constants/constants.hpp>
#include <cmath>

const double PI = boost::math::constants::pi<double>();
const double HALF_PI = PI / 2.0;
const double DEG_TO_RAD_MULT = PI / 180.0;
const double RAD_TO_DEG_MULT = 180.0 / PI;

class Vector3;
class Quaternion;

double degToRad(const double deg);
double radToDeg(const double rad);
void setOpenGLMatrix(float* const m, const Vector3& pos, const Quaternion& rot);

#include "matrix3x3.hpp"

inline double degToRad(const double deg) {
    return deg * DEG_TO_RAD_MULT;
}

inline double radToDeg(const double rad) {
    return rad * RAD_TO_DEG_MULT;
}

inline void setOpenGLMatrix(float*const m, const Vector3& pos, const Quaternion& rot) {
    Matrix3x3 temp(rot);
    m[0]  = static_cast<float>(temp.getRow(0).getX());
    m[1]  = static_cast<float>(temp.getRow(1).getX());
    m[2]  = static_cast<float>(temp.getRow(2).getX());
    m[3]  = 0.0f;
    m[4]  = static_cast<float>(temp.getRow(0).getY());
    m[5]  = static_cast<float>(temp.getRow(1).getY());
    m[6]  = static_cast<float>(temp.getRow(2).getY());
    m[7]  = 0.0f;
    m[8]  = static_cast<float>(temp.getRow(0).getZ());
    m[9]  = static_cast<float>(temp.getRow(1).getZ());
    m[10] = static_cast<float>(temp.getRow(2).getZ());
    m[11] = 0.0f;
    m[12] = static_cast<float>(pos.getX());
    m[13] = static_cast<float>(pos.getY());
    m[14] = static_cast<float>(pos.getZ());
    m[15] = 1.0f;
}

#endif // COMMON_HPP
