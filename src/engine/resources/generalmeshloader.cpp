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

const string OPTIMIZED_BINARY_FILE_EXTENSION = ".bin";

bool GeneralMeshLoader::load(const std::string& fileName, std::vector<Submesh>& submeshes) {
    if (!loadBinary(fileName, submeshes)) {
        if (!import(fileName, submeshes))
            return false;
        writeBinary(fileName, submeshes);
    }
    return true;
}

bool GeneralMeshLoader::import(const std::string& fileName, std::vector<Submesh>& submeshes) {
    submeshes.clear();
    cout << "Importing mesh: " << fileName << endl;

    Assimp::Importer importer;

    //check if file exists
    ifstream fin(fileName.c_str());
    if (!fin.is_open() || !fin.good()) {
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

    submeshes.resize(scene->mNumMeshes);
    for (size_t n = 0; n < submeshes.size(); ++n) {
        const struct aiMesh* mesh = scene->mMeshes[n];

        submeshes[n].vertices.reserve(mesh->mNumVertices * 3);
        for (size_t i = 0; i < mesh->mNumVertices; ++i) {
            submeshes[n].vertices.push_back(mesh->mVertices[i].x);
            submeshes[n].vertices.push_back(mesh->mVertices[i].y);
            submeshes[n].vertices.push_back(mesh->mVertices[i].z);
        }

        submeshes[n].normals.reserve(mesh->mNumVertices * 3);
        for (size_t i = 0; i < mesh->mNumVertices; ++i) {
            submeshes[n].normals.push_back(mesh->mNormals[i].x);
            submeshes[n].normals.push_back(mesh->mNormals[i].y);
            submeshes[n].normals.push_back(mesh->mNormals[i].z);
        }

        submeshes[n].indices.reserve(mesh->mNumFaces * 3);
        for (size_t i = 0; i < mesh->mNumFaces; ++i) {
            if (mesh->mFaces[i].mNumIndices != 3) {
                cerr << "Error: non-triangle face found in file: " << fileName << endl;
                continue;
            }
            submeshes[n].indices.push_back(mesh->mFaces[i].mIndices[0]);
            submeshes[n].indices.push_back(mesh->mFaces[i].mIndices[1]);
            submeshes[n].indices.push_back(mesh->mFaces[i].mIndices[2]);
        }
    }
    return true;
}

bool GeneralMeshLoader::loadBinary(const std::string& fileName, std::vector<Submesh>& submeshes) {
    submeshes.clear();
    string fileBin = fileName + OPTIMIZED_BINARY_FILE_EXTENSION;
    ifstream file(fileBin.c_str(), ios::in | ios::binary);
    if (!file.is_open() || !file.good()) {
        cerr << "Optimized binary file not found: " << fileBin << endl;
        return false;
    }

    // temporal values
    size_t size;
    float fVal;
    unsigned int iVal;

    cout << "Loading mesh: " << fileBin << endl;
    // load header
    file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
    submeshes.resize(size);

    // load body
    for (size_t n = 0; n < submeshes.size(); ++n) {
        // vertices
        file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
        submeshes[n].vertices.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            file.read(reinterpret_cast<char*>(&fVal), sizeof(float));
            submeshes[n].vertices.push_back(fVal);
        }
        // normals
        file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
        submeshes[n].normals.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            file.read(reinterpret_cast<char*>(&fVal), sizeof(float));
            submeshes[n].normals.push_back(fVal);
        }
        // indices
        file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
        submeshes[n].indices.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            file.read(reinterpret_cast<char*>(&iVal), sizeof(unsigned int));
            submeshes[n].indices.push_back(iVal);
        }
    }
    file.close();
    return true;
}

bool GeneralMeshLoader::writeBinary(const std::string& fileName, std::vector<Submesh>& submeshes) {
    string fileBin = fileName + OPTIMIZED_BINARY_FILE_EXTENSION;
    cout << "Saving optimized binary mesh: " << fileBin << endl;
    ofstream file(fileBin.c_str(), ios::out | ios::binary | ios::trunc);
    if (!file.is_open() || !file.good()) {
        cerr << "Error: could not open file: " << fileBin << endl;
        return false;
    }

    // temporal values
    size_t size;
    float fVal;
    unsigned int iVal;

    // write header
    size = submeshes.size();
    file.write(reinterpret_cast<char*>(&size), sizeof(size_t));

    // write body
    for (size_t n = 0; n < submeshes.size(); ++n) {
        // vertices
        size = submeshes[n].vertices.size();
        file.write(reinterpret_cast<char*>(&size), sizeof(size_t));
        for (size_t i = 0; i < submeshes[n].vertices.size(); ++i) {
            fVal = submeshes[n].vertices[i];
            file.write(reinterpret_cast<char*>(&fVal), sizeof(float));
        }
        // normals
        size = submeshes[n].normals.size();
        file.write(reinterpret_cast<char*>(&size), sizeof(size_t));
        for (size_t i = 0; i < submeshes[n].normals.size(); ++i) {
            fVal = submeshes[n].normals[i];
            file.write(reinterpret_cast<char*>(&fVal), sizeof(float));
        }
        // indices
        size = submeshes[n].indices.size();
        file.write(reinterpret_cast<char*>(&size), sizeof(size_t));
        for (size_t i = 0; i < submeshes[n].indices.size(); ++i) {
            iVal = submeshes[n].indices[i];
            file.write(reinterpret_cast<char*>(&iVal), sizeof(unsigned int));
        }
    }
    file.close();
    return true;
}
