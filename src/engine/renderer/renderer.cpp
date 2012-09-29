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
#include "engine/renderer/rendermanager.hpp"
#include <engine/renderer/camera.hpp>
#include <engine/renderer/renderablemesh.hpp>

using namespace std;

const GLint VERTEX_TUPLE_SIZE = 3;

Renderer::Renderer() {
    cout << "Detecting OpenGL version" << endl;
    cout << "Using OpenGL Legacy" << endl;
}

void Renderer::initialize() {
    GLint dims[4];
    glGetIntegerv(GL_VIEWPORT, dims);
    RenderManager::ms_activeCamera->setViewport(dims[0], dims[1], dims[2], dims[3]);

    glFrontFace(GL_CCW); // redundant
    glCullFace(GL_BACK); // redundant
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    updateProjectionMatrix();

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

void Renderer::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float lightPosition[] = {-2.0f, 2.0f, -3.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glTranslatef(0.0f, 0.0f, -5.0f);
    set<RenderableMesh*>::const_iterator it;
    for (it = RenderManager::ms_meshes.begin(); it != RenderManager::ms_meshes.end(); ++it) {
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(VERTEX_TUPLE_SIZE, GL_FLOAT, 0, &(*it)->getVertices()[0]);
        glNormalPointer(GL_FLOAT, 0, &(*it)->getNormals()[0]);

        glPushMatrix();
        glDrawElements(GL_TRIANGLES, (*it)->getIndices().size(), GL_UNSIGNED_BYTE, &(*it)->getIndices()[0]);
        glPopMatrix();

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
    }
}

void Renderer::updateProjectionMatrix() {
    Camera* camera = RenderManager::ms_activeCamera;
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
}
