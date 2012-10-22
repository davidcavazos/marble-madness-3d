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


#ifndef MODELLOADER_HPP
#define MODELLOADER_HPP

#include "model.hpp"

class ModelLoader {
public:
    static bool load(const std::string& fileName, Model& model);

private:
    static bool import(const std::string& fileName, Model& model);
    static bool loadBinary(const std::string& fileName, Model& model);
    static bool writeBinary(const std::string& fileName, Model& model);
};

#endif // MODELLOADER_HPP
