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


#include "engine/resources/modelloader.hpp"

#include <iostream>
#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

const string OPTIMIZED_BINARY_FILE_EXTENSION = ".bin";

bool ModelLoader::load(const std::string& fileName, Model& model) {
    if (!loadBinary(fileName, model)) {
        if (!import(fileName, model))
            return false;
        writeBinary(fileName, model);
    }
    return true;
}

bool ModelLoader::import(const std::string& fileName, Model& model) {
    model.m_meshes.clear();
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

    model.m_meshes.resize(scene->mNumMeshes);
    for (size_t n = 0; n < model.getTotalMeshes(); ++n) {
        const struct aiMesh* mesh = scene->mMeshes[n];

        // vertices
        model.mesh(n).m_vertices.reserve(mesh->mNumVertices * 3);
        for (size_t i = 0; i < mesh->mNumVertices; ++i) {
            model.mesh(n).m_vertices.push_back(mesh->mVertices[i].x);
            model.mesh(n).m_vertices.push_back(mesh->mVertices[i].y);
            model.mesh(n).m_vertices.push_back(mesh->mVertices[i].z);
        }

        // normals
        model.mesh(n).m_normals.reserve(mesh->mNumVertices * 3);
        for (size_t i = 0; i < mesh->mNumVertices; ++i) {
            model.mesh(n).m_normals.push_back(mesh->mNormals[i].x);
            model.mesh(n).m_normals.push_back(mesh->mNormals[i].y);
            model.mesh(n).m_normals.push_back(mesh->mNormals[i].z);
        }

        // indices
        model.mesh(n).m_indices.reserve(mesh->mNumFaces * 3);
        for (size_t i = 0; i < mesh->mNumFaces; ++i) {
            if (mesh->mFaces[i].mNumIndices != 3) {
                cerr << "Error: non-triangle face found, check model: " << fileName << endl;
                continue;
            }
            model.mesh(n).m_indices.push_back(mesh->mFaces[i].mIndices[0]);
            model.mesh(n).m_indices.push_back(mesh->mFaces[i].mIndices[1]);
            model.mesh(n).m_indices.push_back(mesh->mFaces[i].mIndices[2]);
        }

        // materials
        model.mesh(n).m_materials.resize(mesh->GetNumUVChannels());
        for (size_t mat = 0; mat < mesh->GetNumUVChannels(); ++mat) {
            // uv coords
            model.mesh(n).material(mat).m_uvCoords.reserve(mesh->mNumVertices * 2);
            for (size_t i = 0; i < mesh->mNumVertices; ++i) {
                model.mesh(n).material(mat).m_uvCoords.push_back(mesh->mTextureCoords[mat][i].x);
                model.mesh(n).material(mat).m_uvCoords.push_back(mesh->mTextureCoords[mat][i].y);
            }
        }
    }
    return true;
}

bool ModelLoader::loadBinary(const std::string& fileName, Model& model) {
    model.m_meshes.clear();
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
    model.m_meshes.resize(size);

    // load body
    for (size_t n = 0; n < model.m_meshes.size(); ++n) {
        // vertices
        file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
        model.mesh(n).m_vertices.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            file.read(reinterpret_cast<char*>(&fVal), sizeof(float));
            model.mesh(n).m_vertices.push_back(fVal);
        }
        // normals
        file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
        model.mesh(n).m_normals.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            file.read(reinterpret_cast<char*>(&fVal), sizeof(float));
            model.mesh(n).m_normals.push_back(fVal);
        }
        // indices
        file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
        model.mesh(n).m_indices.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            file.read(reinterpret_cast<char*>(&iVal), sizeof(unsigned int));
            model.mesh(n).m_indices.push_back(iVal);
        }
        // materials
        file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
        model.mesh(n).m_materials.resize(size);
        for (size_t mat = 0; mat < model.mesh(n).getTotalMaterials(); ++mat) {
            // uv coords
            file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
            model.mesh(n).material(mat).m_uvCoords.reserve(size);
            for (size_t i = 0; i < size; ++i) {
                file.read(reinterpret_cast<char*>(&fVal), sizeof(float));
                model.mesh(n).material(mat).m_uvCoords.push_back(fVal);
            }
        }
    }
    file.close();
    return true;
}

bool ModelLoader::writeBinary(const std::string& fileName, Model& model) {
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
    size = model.getTotalMeshes();
    file.write(reinterpret_cast<char*>(&size), sizeof(size_t));

    // write body
    for (size_t n = 0; n < model.m_meshes.size(); ++n) {
        // vertices
        size = model.mesh(n).getTotalVertices();
        file.write(reinterpret_cast<char*>(&size), sizeof(size_t));
        for (size_t i = 0; i < model.mesh(n).getTotalVertices(); ++i) {
            fVal = model.mesh(n).m_vertices[i];
            file.write(reinterpret_cast<char*>(&fVal), sizeof(float));
        }
        // normals
        size = model.mesh(n).getTotalNormals();
        file.write(reinterpret_cast<char*>(&size), sizeof(size_t));
        for (size_t i = 0; i < model.mesh(n).getTotalNormals(); ++i) {
            fVal = model.mesh(n).m_normals[i];
            file.write(reinterpret_cast<char*>(&fVal), sizeof(float));
        }
        // indices
        size = model.mesh(n).getTotalIndices();
        file.write(reinterpret_cast<char*>(&size), sizeof(size_t));
        for (size_t i = 0; i < model.mesh(n).getTotalIndices(); ++i) {
            iVal = model.mesh(n).m_indices[i];
            file.write(reinterpret_cast<char*>(&iVal), sizeof(unsigned int));
        }
        // materials
        size = model.mesh(n).getTotalMaterials();
        file.write(reinterpret_cast<char*>(&size), sizeof(size_t));
        for (size_t mat = 0; mat < model.mesh(n).getTotalMaterials(); ++mat) {
            // uv coords
            size = model.mesh(n).material(mat).getTotalUvCoords();
            file.write(reinterpret_cast<char*>(&size), sizeof(size_t));
            for (size_t i = 0; i < size; ++i) {
                fVal = model.mesh(n).material(mat).m_uvCoords[i];
                file.write(reinterpret_cast<char*>(&fVal), sizeof(float));
            }
        }
    }
    file.close();
    return true;
}
