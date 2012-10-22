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

bool GeneralMeshLoader::load(const std::string& fileName, std::vector<Mesh>& meshes) {
    if (!loadBinary(fileName, meshes)) {
        if (!import(fileName, meshes))
            return false;
        writeBinary(fileName, meshes);
    }
    return true;
}

bool GeneralMeshLoader::import(const std::string& fileName, std::vector<Mesh>& meshes) {
    meshes.clear();
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

    meshes.resize(scene->mNumMeshes);
    for (size_t n = 0; n < meshes.size(); ++n) {
        const struct aiMesh* mesh = scene->mMeshes[n];

        meshes[n].vertices.reserve(mesh->mNumVertices * 3);
        for (size_t i = 0; i < mesh->mNumVertices; ++i) {
            meshes[n].vertices.push_back(mesh->mVertices[i].x);
            meshes[n].vertices.push_back(mesh->mVertices[i].y);
            meshes[n].vertices.push_back(mesh->mVertices[i].z);
        }

        meshes[n].normals.reserve(mesh->mNumVertices * 3);
        for (size_t i = 0; i < mesh->mNumVertices; ++i) {
            meshes[n].normals.push_back(mesh->mNormals[i].x);
            meshes[n].normals.push_back(mesh->mNormals[i].y);
            meshes[n].normals.push_back(mesh->mNormals[i].z);
        }

        meshes[n].indices.reserve(mesh->mNumFaces * 3);
        for (size_t i = 0; i < mesh->mNumFaces; ++i) {
            if (mesh->mFaces[i].mNumIndices != 3) {
                cerr << "Error: non-triangle face found, check model: " << fileName << endl;
                continue;
            }
            meshes[n].indices.push_back(mesh->mFaces[i].mIndices[0]);
            meshes[n].indices.push_back(mesh->mFaces[i].mIndices[1]);
            meshes[n].indices.push_back(mesh->mFaces[i].mIndices[2]);
        }

        meshes[n].uvCoords.resize(mesh->GetNumUVChannels());
        for (size_t uv = 0; uv < mesh->GetNumUVChannels(); ++uv) {
            meshes[n].uvCoords[uv].reserve(mesh->mNumVertices * 2);
            for (size_t i = 0; i < mesh->mNumVertices; ++i) {
                meshes[n].uvCoords[uv].push_back(mesh->mTextureCoords[uv][i].x);
                meshes[n].uvCoords[uv].push_back(mesh->mTextureCoords[uv][i].y);
            }
        }
    }
    return true;
}

bool GeneralMeshLoader::loadBinary(const std::string& fileName, std::vector<Mesh>& meshes) {
    meshes.clear();
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
    meshes.resize(size);

    // load body
    for (size_t n = 0; n < meshes.size(); ++n) {
        // vertices
        file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
        meshes[n].vertices.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            file.read(reinterpret_cast<char*>(&fVal), sizeof(float));
            meshes[n].vertices.push_back(fVal);
        }
        // normals
        file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
        meshes[n].normals.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            file.read(reinterpret_cast<char*>(&fVal), sizeof(float));
            meshes[n].normals.push_back(fVal);
        }
        // indices
        file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
        meshes[n].indices.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            file.read(reinterpret_cast<char*>(&iVal), sizeof(unsigned int));
            meshes[n].indices.push_back(iVal);
        }
        // uv coords
        file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
        meshes[n].uvCoords.resize(size);
        for (size_t uv = 0; uv < meshes[n].uvCoords.size(); ++uv) {
            file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
            meshes[n].uvCoords[uv].reserve(size);
            for (size_t i = 0; i < size; ++i) {
                file.read(reinterpret_cast<char*>(&fVal), sizeof(float));
                meshes[n].uvCoords[uv].push_back(fVal);
            }
        }
    }
    file.close();
    return true;
}

bool GeneralMeshLoader::writeBinary(const std::string& fileName, std::vector<Mesh>& submeshes) {
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
        // uv coords
        size = submeshes[n].uvCoords.size();
        file.write(reinterpret_cast<char*>(&size), sizeof(size_t));
        for (size_t uv = 0; uv < submeshes[n].uvCoords.size(); ++uv) {
            size = submeshes[n].uvCoords[uv].size();
            file.write(reinterpret_cast<char*>(&size), sizeof(size_t));
            for (size_t i = 0; i < size; ++i) {
                fVal = submeshes[n].uvCoords[uv][i];
                file.write(reinterpret_cast<char*>(&fVal), sizeof(float));
            }
        }
    }
    file.close();
    return true;
}
