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


#ifndef SCALAR_HPP
#define SCALAR_HPP

#include <cfloat>

typedef double scalar_t;

// const scalar_t EPSILON = FLT_EPSILON;
const scalar_t EPSILON = DBL_EPSILON;
const scalar_t ZERO = static_cast<scalar_t>(0.0);
const scalar_t ONE = static_cast<scalar_t>(1.0);
const scalar_t HALF = static_cast<scalar_t>(0.5);
const scalar_t TWO = static_cast<scalar_t>(2.0);

#endif // SCALAR_HPP
