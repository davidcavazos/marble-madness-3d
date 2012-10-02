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
#include "engine/renderer/rendermanager.hpp"
#include <engine/renderer/camera.hpp>
#include <engine/renderer/renderablemesh.hpp>

using namespace std;

const GLint VERTEX_TUPLE_SIZE = 3;

void Renderer::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float lightPosition[] = {-2.0f, 2.0f, -3.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    Transform& camTrans = RenderManager::ms_activeCamera->getEntity().getTransform();
    glRotatef(radToDeg(camTrans.calcPitch()), 1.0f, 0.0f, 0.0f);
    glRotatef(radToDeg(camTrans.calcYaw()), 0.0f, 1.0f, 0.0f);
    glRotatef(radToDeg(camTrans.calcRoll()), 0.0f, 0.0f, 1.0f);
    glTranslatef(-camTrans.getPosition().getX(), -camTrans.getPosition().getY(), -camTrans.getPosition().getZ());

    set<RenderableMesh*>::const_iterator it;
    for (it = RenderManager::ms_meshes.begin(); it != RenderManager::ms_meshes.end(); ++it) {
        Transform& trans = (*it)->getEntity().getTransform();

        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(VERTEX_TUPLE_SIZE, GL_FLOAT, 0, &(*it)->getVertices()[0]);
        glNormalPointer(GL_FLOAT, 0, &(*it)->getNormals()[0]);

        glPushMatrix();
        glTranslatef(trans.getPosition().getX(), trans.getPosition().getY(), trans.getPosition().getZ());
        glRotatef(radToDeg(trans.calcPitch()), 1.0f, 0.0f, 0.0f);
        glRotatef(radToDeg(trans.calcYaw()), 0.0f, 1.0f, 0.0f);
        glRotatef(radToDeg(trans.calcRoll()), 0.0f, 0.0f, 1.0f);
        glDrawElements(GL_TRIANGLES, (*it)->getIndices().size(), GL_UNSIGNED_BYTE, &(*it)->getIndices()[0]);
        glPopMatrix();

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
    }
}

Renderer::Renderer() {
    cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
    cout << "Shader language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
    cout << "Vendor: " << glGetString(GL_VENDOR) << endl;
    cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
    cout << "Using OpenGL Legacy" << endl;
}

void Renderer::initialize() {
    glFrontFace(GL_CCW); // redundant
    glCullFace(GL_BACK); // redundant
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

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
