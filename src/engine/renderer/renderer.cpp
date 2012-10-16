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
#include <GL/glu.h>
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

void Renderer::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float lightPosition[] = {-2.0f, 2.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    float m[16];

    Entity& cam = RenderManager::ms_activeCamera->getEntity();
    setOpenGLMatrix(m, VECTOR3_ZERO, cam.getOrientationAbs().inverse());
    glMultMatrixf(m);
    glTranslatef(-cam.getPositionAbs().getX(), -cam.getPositionAbs().getY(), -cam.getPositionAbs().getZ());

    set<RenderableMesh*>::const_iterator it;
    for (it = RenderManager::ms_meshes.begin(); it != RenderManager::ms_meshes.end(); ++it) {
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

Renderer::Renderer() {
    cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
    cout << "Shader language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
    cout << "Vendor: " << glGetString(GL_VENDOR) << endl;
    cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
    cout << "Using OpenGL Legacy" << endl;

//     GLint maxElements = 0;
//     glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &maxElements);
//     cout << "Vertex limit: " << maxElements << endl;
//     glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &maxElements);
//     cout << "Index limit: " << maxElements << endl;
}

void Renderer::initialize() {
    glFrontFace(GL_CCW); // redundant
    glCullFace(GL_BACK); // redundant
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    Camera* camera = RenderManager::ms_activeCamera;

    Device& dev = DeviceManager::getDevice();
    camera->setViewport(0, 0, dev.getWinWidth(), dev.getWinHeight());
    viewport_t view = camera->getViewport();
    glViewport(view.posX, view.posY, view.width, view.height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    switch (camera->getType()) {
        case CAMERA_ORTHOGRAPHIC:
            glOrtho(
                -camera->getOrthoWidth(),
                    camera->getOrthoWidth(),
                    -camera->getOrthoHeight(),
                    camera->getOrthoHeight(),
                    camera->getNearDistance(),
                    camera->getFarDistance()
            );
            break;
        case CAMERA_PROJECTION:
            gluPerspective(
                camera->getPerspectiveFOV(),
                           camera->getAspectRatio(),
                           camera->getNearDistance(),
                           camera->getFarDistance()
            );
            break;
    }
    glMatrixMode(GL_MODELVIEW);

    GLfloat global_ambient[] = {0.5f, 0.5f, 1.0f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    glEnable(GL_LIGHT0);
    float diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    float ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

}

void Renderer::deinitialize() {
}
