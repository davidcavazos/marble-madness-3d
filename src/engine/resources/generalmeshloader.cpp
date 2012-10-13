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


#include "engine/resources/generalmeshloader.hpp"

#include <iostream>
#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

bool GeneralMeshLoader::load(const std::string& fileName,
                             std::vector<float>& vertices,
                             std::vector<float>& normals,
                             std::vector<unsigned char>& indices)
{
    import(fileName, vertices, normals, indices);
    return true;
}

bool GeneralMeshLoader::import(const std::string& fileName,
                                    std::vector<float>& vertices,
                                    std::vector<float>& normals,
                                    std::vector<unsigned char>& indices)
{
    vertices.clear();
    normals.clear();
    indices.clear();
    cout << "Importing mesh: " << fileName << endl;

    Assimp::Importer importer;

    //check if file exists
    std::ifstream fin(fileName.c_str());
    if(!fin.good()) {
        cerr << "Error: could not open file: " << fileName << endl;
        cerr << importer.GetErrorString() << endl;
        return false;
    }
    fin.close();

    const struct aiScene* scene;
    scene = importer.ReadFile(fileName.c_str(),
                              aiProcess_CalcTangentSpace |
                              aiProcess_GenSmoothNormals |
                              aiProcess_JoinIdenticalVertices |
                              aiProcess_ImproveCacheLocality |
                              aiProcess_LimitBoneWeights |
                              aiProcess_RemoveRedundantMaterials |
                              aiProcess_SplitLargeMeshes |
                              aiProcess_Triangulate |
                              aiProcess_GenUVCoords |
                              aiProcess_SortByPType |
                              aiProcess_FindDegenerates |
                              aiProcess_FindInvalidData |
                              aiProcess_FindInstances |
                              aiProcess_ValidateDataStructure |
                              aiProcess_OptimizeMeshes |
                              aiProcess_Debone);
    if (scene == 0) {
        cerr << importer.GetErrorString() << endl;
        return false;
    }

    const struct aiMesh* mesh = scene->mMeshes[0];
    vertices.reserve(mesh->mNumVertices * 3);
    for (size_t i = 0; i < mesh->mNumVertices; ++i) {
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);
    }
    if (mesh->HasNormals()) {
        normals.reserve(mesh->mNumVertices * 3);
        for (size_t i = 0; i < mesh->mNumVertices; ++i) {
            normals.push_back(mesh->mNormals[i].x);
            normals.push_back(mesh->mNormals[i].y);
            normals.push_back(mesh->mNormals[i].z);
        }
    }
    if (mesh->HasFaces()) {
        indices.reserve(mesh->mNumFaces * 3);
        for (size_t i = 0; i < mesh->mNumFaces; ++i) {
            if (mesh->mFaces[i].mNumIndices != 3)
                cerr << "Error: non-triangle face found in file: " << fileName << endl;
            indices.push_back(mesh->mFaces[i].mIndices[0]);
            indices.push_back(mesh->mFaces[i].mIndices[1]);
            indices.push_back(mesh->mFaces[i].mIndices[2]);
        }
    }
    return true;
}

bool GeneralMeshLoader::loadBinary(const std::string& fileName,
                                   std::vector<float>& vertices,
                                   std::vector<float>& normals,
                                   std::vector<unsigned char>& indices)
{
    vertices.clear();
    normals.clear();
    indices.clear();
    cout << "Loading mesh: " << fileName << endl;
    return true;
}

bool GeneralMeshLoader::writeBinary(const std::string& fileName,
                                    std::vector<float>& vertices,
                                    std::vector<float>& normals,
                                    std::vector<unsigned char>& indices)
{
    cout << "Saving optimized binary mesh: " << fileName << endl;
    return true;
}
