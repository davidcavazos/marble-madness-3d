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
const double DEG_TO_RAD_MULT = PI / 180.0;
const double RAD_TO_DEG_MULT = 180.0 / PI;



double degToRad(const double deg);
double radToDeg(const double rad);



inline double degToRad(const double deg) {
    return deg * DEG_TO_RAD_MULT;
}

inline double radToDeg(const double rad) {
    return rad * RAD_TO_DEG_MULT;
}

#endif // COMMON_HPP
