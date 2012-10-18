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


#include "engine/renderer/renderer.hpp"

#include <iostream>
#include <GL/glew.h>
#include "engine/kernel/common.hpp"
#include "engine/kernel/device.hpp"
#include "engine/kernel/devicemanager.hpp"
#include "engine/kernel/entity.hpp"
#include "engine/kernel/matrix3x3.hpp"
#include "engine/renderer/rendermanager.hpp"
#include "engine/renderer/camera.hpp"
#include "engine/renderer/renderablemesh.hpp"
#include "engine/resources/meshdata.hpp"

using namespace std;

void Renderer::initCamera() const {
    Device& dev = DeviceManager::getDevice();
    m_activeCamera->setViewport(0, 0, dev.getWinWidth(), dev.getWinHeight());
    viewport_t view = m_activeCamera->getViewport();
    glViewport(view.posX, view.posY, view.width, view.height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    switch (m_activeCamera->getType()) {
        case CAMERA_ORTHOGRAPHIC:
            glOrtho(
                -m_activeCamera->getOrthoWidth(),
                    m_activeCamera->getOrthoWidth(),
                    -m_activeCamera->getOrthoHeight(),
                    m_activeCamera->getOrthoHeight(),
                    m_activeCamera->getNearDistance(),
                    m_activeCamera->getFarDistance()
            );
            break;
        case CAMERA_PROJECTION:
            gluPerspective(
                m_activeCamera->getPerspectiveFOV(),
                           m_activeCamera->getAspectRatio(),
                           m_activeCamera->getNearDistance(),
                           m_activeCamera->getFarDistance()
            );
            break;
    }
    glMatrixMode(GL_MODELVIEW);
}

void Renderer::draw() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float m[16];

    Entity& cam = m_activeCamera->getEntity();
    setOpenGLMatrix(m, VECTOR3_ZERO, cam.getOrientationAbs().inverse());
    glMultMatrixf(m);
    glTranslatef(-cam.getPositionAbs().getX(), -cam.getPositionAbs().getY(), -cam.getPositionAbs().getZ());

    set<RenderableMesh*>::const_iterator it;
    for (it = m_meshes.begin(); it != m_meshes.end(); ++it) {
        const MeshData& mesh = (*it)->getMeshData();
        const Entity& entity = (*it)->getEntity();

        glPushMatrix();
        setOpenGLMatrix(m, entity.getPositionAbs(), entity.getOrientationAbs());
        glMultMatrixf(m);
        for (size_t submesh = 0; submesh < mesh.getTotalSubmeshes(); ++submesh) {
            glVertexPointer(3, GL_FLOAT, 0, mesh.getVerticesPtr(submesh));
            glNormalPointer(GL_FLOAT, 0, mesh.getNormalsPtr(submesh));
            glDrawElements(GL_TRIANGLES, mesh.getIndices(submesh).size(), GL_UNSIGNED_INT, mesh.getIndicesPtr(submesh));
        }
        glPopMatrix();
    }
}

Renderer::Renderer():
    m_activeCamera(0),
    m_cameras(),
    m_meshes()
{
    cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
    cout << "Shader language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
    cout << "Vendor: " << glGetString(GL_VENDOR) << endl;
    cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
    cout << "Using OpenGL Legacy" << endl;

    GLint numExtensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
    cout << numExtensions << " extensions" << endl;

//     GLint maxElements = 0;
//     glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &maxElements);
//     cout << "Vertex limit: " << maxElements << endl;
//     glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &maxElements);
//     cout << "Index limit: " << maxElements << endl;
}

Renderer::Renderer(const Renderer& rhs):
    m_activeCamera(rhs.m_activeCamera),
    m_cameras(rhs.m_cameras),
    m_meshes(rhs.m_meshes)
{
    cerr << "Renderer copy constructor should not be called" << endl;
}

Renderer& Renderer::operator=(const Renderer& rhs) {
    cerr << "Renderer assignment operator should not be called" << endl;
    if (this == &rhs)
        return *this;
    m_activeCamera = rhs.m_activeCamera;
    m_cameras = rhs.m_cameras;
    m_meshes = rhs.m_meshes;
    return *this;
}

void Renderer::initialize() {
    // always
    glFrontFace(GL_CCW); // redundant
    glCullFace(GL_BACK); // redundant
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
//     glShadeModel(GL_SMOOTH); // using manually defined normals

    // enable lighting for legacy lights
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    glEnable(GL_LIGHT4);
    glEnable(GL_LIGHT5);
    glEnable(GL_LIGHT6);
    glEnable(GL_LIGHT7);
    GLfloat global_ambient[] = {0.5f, 0.5f, 1.0f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    float diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    float ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    float lightPosition[] = {-2.0f, 2.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    // enable arrays for Vertex Array (Legacy)
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
}

void Renderer::deinitialize() {
    set<Camera*>::const_iterator itCam;
    for (itCam = m_cameras.begin(); itCam != m_cameras.end(); ++itCam)
        delete *itCam;

    set<RenderableMesh*>::const_iterator itMesh;
    for (itMesh = m_meshes.begin(); itMesh != m_meshes.end(); ++itMesh)
        delete *itMesh;
}

string Renderer::listsToString() const {
    stringstream ss;
    ss << "Renderer Cameras List:" << endl;
    set<Camera*>::const_iterator itCam;
    for (itCam = m_cameras.begin(); itCam != m_cameras.end(); ++itCam) {
        ss << "  " << (*itCam)->getDescription();
        if (*itCam == m_activeCamera)
            ss << " *";
        ss << endl;
    }
    ss << endl;

    ss << "Renderer Meshes List:" << endl;
    set<RenderableMesh*>::const_iterator itMesh;
    for (itMesh = m_meshes.begin(); itMesh != m_meshes.end(); ++itMesh)
        ss << "  " << (*itMesh)->getDescription() << endl;

    return ss.str();
}
