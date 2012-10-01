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


#include "engine/renderer/renderablemesh.hpp"

#include <sstream>
#include "engine/renderer/rendermanager.hpp"

using namespace std;

const string CUBE_DESCRIPTION = "$cube";

RenderableMesh::RenderableMesh(Entity* const entity):
    Component(COMPONENT_RENDERABLE_MESH, entity),
    m_isInitialized(false),
    m_vertices(),
    m_normals(),
    m_indices()
{
    RenderManager::ms_meshes.insert(this);
}

RenderableMesh::~RenderableMesh() {
    RenderManager::ms_meshes.erase(this);
}

void RenderableMesh::generateCube(const double lengthX, const double lengthY, const double lengthZ) {
    double x = lengthX / 2.0;
    double y = lengthY / 2.0;
    double z = lengthZ / 2.0;

    stringstream ss;
    ss << CUBE_DESCRIPTION << "_" << lengthX << "_" << lengthY << "_" << lengthZ;
    m_description = ss.str();

    m_vertices.clear();
    m_vertices.reserve(72);
    // Front
    m_vertices.push_back( x); m_vertices.push_back( y); m_vertices.push_back( z);
    m_vertices.push_back(-x); m_vertices.push_back( y); m_vertices.push_back( z);
    m_vertices.push_back(-x); m_vertices.push_back(-y); m_vertices.push_back( z);
    m_vertices.push_back( x); m_vertices.push_back(-y); m_vertices.push_back( z);
    // Right
    m_vertices.push_back( x); m_vertices.push_back( y); m_vertices.push_back( z);
    m_vertices.push_back( x); m_vertices.push_back(-y); m_vertices.push_back( z);
    m_vertices.push_back( x); m_vertices.push_back(-y); m_vertices.push_back(-z);
    m_vertices.push_back( x); m_vertices.push_back( y); m_vertices.push_back(-z);
    // Top
    m_vertices.push_back( x); m_vertices.push_back( y); m_vertices.push_back( z);
    m_vertices.push_back( x); m_vertices.push_back( y); m_vertices.push_back(-z);
    m_vertices.push_back(-x); m_vertices.push_back( y); m_vertices.push_back(-z);
    m_vertices.push_back(-x); m_vertices.push_back( y); m_vertices.push_back( z);
    // Left
    m_vertices.push_back(-x); m_vertices.push_back( y); m_vertices.push_back( z);
    m_vertices.push_back(-x); m_vertices.push_back( y); m_vertices.push_back(-z);
    m_vertices.push_back(-x); m_vertices.push_back(-y); m_vertices.push_back(-z);
    m_vertices.push_back(-x); m_vertices.push_back(-y); m_vertices.push_back( z);
    // Bottom
    m_vertices.push_back(-x); m_vertices.push_back(-y); m_vertices.push_back(-z);
    m_vertices.push_back( x); m_vertices.push_back(-y); m_vertices.push_back(-z);
    m_vertices.push_back( x); m_vertices.push_back(-y); m_vertices.push_back( z);
    m_vertices.push_back(-x); m_vertices.push_back(-y); m_vertices.push_back( z);
    // Back
    m_vertices.push_back( x); m_vertices.push_back(-y); m_vertices.push_back(-z);
    m_vertices.push_back(-x); m_vertices.push_back(-y); m_vertices.push_back(-z);
    m_vertices.push_back(-x); m_vertices.push_back( y); m_vertices.push_back(-z);
    m_vertices.push_back( x); m_vertices.push_back( y); m_vertices.push_back(-z);

    m_normals.clear();
    m_normals.reserve(72);
    // Front
    m_normals.push_back( 0.0); m_normals.push_back( 0.0); m_normals.push_back( 1.0);
    m_normals.push_back( 0.0); m_normals.push_back( 0.0); m_normals.push_back( 1.0);
    m_normals.push_back( 0.0); m_normals.push_back( 0.0); m_normals.push_back( 1.0);
    m_normals.push_back( 0.0); m_normals.push_back( 0.0); m_normals.push_back( 1.0);
    // Right
    m_normals.push_back( 1.0); m_normals.push_back( 0.0); m_normals.push_back( 0.0);
    m_normals.push_back( 1.0); m_normals.push_back( 0.0); m_normals.push_back( 0.0);
    m_normals.push_back( 1.0); m_normals.push_back( 0.0); m_normals.push_back( 0.0);
    m_normals.push_back( 1.0); m_normals.push_back( 0.0); m_normals.push_back( 0.0);
    // Top
    m_normals.push_back( 0.0); m_normals.push_back( 1.0); m_normals.push_back( 0.0);
    m_normals.push_back( 0.0); m_normals.push_back( 1.0); m_normals.push_back( 0.0);
    m_normals.push_back( 0.0); m_normals.push_back( 1.0); m_normals.push_back( 0.0);
    m_normals.push_back( 0.0); m_normals.push_back( 1.0); m_normals.push_back( 0.0);
    // Left
    m_normals.push_back(-1.0); m_normals.push_back( 0.0); m_normals.push_back( 0.0);
    m_normals.push_back(-1.0); m_normals.push_back( 0.0); m_normals.push_back( 0.0);
    m_normals.push_back(-1.0); m_normals.push_back( 0.0); m_normals.push_back( 0.0);
    m_normals.push_back(-1.0); m_normals.push_back( 0.0); m_normals.push_back( 0.0);
    // Bottom
    m_normals.push_back( 0.0); m_normals.push_back(-1.0); m_normals.push_back( 0.0);
    m_normals.push_back( 0.0); m_normals.push_back(-1.0); m_normals.push_back( 0.0);
    m_normals.push_back( 0.0); m_normals.push_back(-1.0); m_normals.push_back( 0.0);
    m_normals.push_back( 0.0); m_normals.push_back(-1.0); m_normals.push_back( 0.0);
    // Back
    m_normals.push_back( 0.0); m_normals.push_back( 0.0); m_normals.push_back(-1.0);
    m_normals.push_back( 0.0); m_normals.push_back( 0.0); m_normals.push_back(-1.0);
    m_normals.push_back( 0.0); m_normals.push_back( 0.0); m_normals.push_back(-1.0);
    m_normals.push_back( 0.0); m_normals.push_back( 0.0); m_normals.push_back(-1.0);

    m_indices.clear();
    m_indices.reserve(36);
    // Front
    m_indices.push_back( 0); m_indices.push_back( 1); m_indices.push_back( 2);
    m_indices.push_back( 2); m_indices.push_back( 3); m_indices.push_back( 0);
    // Right
    m_indices.push_back( 4); m_indices.push_back( 5); m_indices.push_back( 6);
    m_indices.push_back( 6); m_indices.push_back( 7); m_indices.push_back( 4);
    // Top
    m_indices.push_back( 8); m_indices.push_back( 9); m_indices.push_back(10);
    m_indices.push_back(10); m_indices.push_back(11); m_indices.push_back( 8);
    // Left
    m_indices.push_back(12); m_indices.push_back(13); m_indices.push_back(14);
    m_indices.push_back(14); m_indices.push_back(15); m_indices.push_back(12);
    // Bottom
    m_indices.push_back(16); m_indices.push_back(17); m_indices.push_back(18);
    m_indices.push_back(18); m_indices.push_back(19); m_indices.push_back(16);
    // Back
    m_indices.push_back(20); m_indices.push_back(21); m_indices.push_back(22);
    m_indices.push_back(22); m_indices.push_back(23); m_indices.push_back(20);

    m_isInitialized = true;
}
