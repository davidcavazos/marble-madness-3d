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
#include "engine/kernel/device.hpp"
#include "engine/kernel/devicemanager.hpp"
#include "engine/kernel/entity.hpp"
#include "engine/kernel/matrix3x3.hpp"
#include "engine/renderer/rendermanager.hpp"
#include "engine/renderer/camera.hpp"
#include "engine/renderer/renderablemesh.hpp"
#include "engine/renderer/light.hpp"
#include "engine/resources/model.hpp"

using namespace std;

void Renderer::setAmbientLight(const float r, const float g, const float b, const float a) {
    GLfloat global_ambient[] = {r, g, b, a};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
}

void Renderer::initLighting() const {
    // enable lighting for legacy lights
//     glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

    set<Light*>::const_iterator it = m_lights.begin();
    for (size_t i = 0; i < m_lights.size(); ++i) {
        GLenum lightEnum;
        switch (i) {
        case 0:
            lightEnum = GL_LIGHT0;
            break;
        case 1:
            lightEnum = GL_LIGHT1;
            break;
        case 2:
            lightEnum = GL_LIGHT2;
            break;
        case 3:
            lightEnum = GL_LIGHT3;
            break;
        case 4:
            lightEnum = GL_LIGHT4;
            break;
        case 5:
            lightEnum = GL_LIGHT5;
            break;
        case 6:
            lightEnum = GL_LIGHT6;
            break;
        case 7: default:
            lightEnum = GL_LIGHT7;
            break;
        }
        glEnable(lightEnum);
        glLightfv(lightEnum, GL_AMBIENT, (*it)->getAmbient());
        glLightfv(lightEnum, GL_DIFFUSE, (*it)->getDiffuse());
        glLightfv(lightEnum, GL_SPECULAR, (*it)->getSpecular());
        ++it;
    }
}

void Renderer::loadTexture(unsigned int& textureId,
                           const size_t bytesPerPixel,
                           const size_t width,
                           const size_t height,
                           const texture_format_t& textureFormat,
                           void* pixels) {
    GLenum textureFormatGL;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glPixelStorei(GL_UNPACK_ALIGNMENT, bytesPerPixel);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    switch (textureFormat) {
    case TEXTURE_FORMAT_RGBA:
        textureFormatGL = GL_RGBA;
        break;
    case TEXTURE_FORMAT_RGB:
        textureFormatGL = GL_RGB;
        break;
    case TEXTURE_FORMAT_BGRA:
        textureFormatGL = GL_BGRA;
        break;
    case TEXTURE_FORMAT_BGR:
        textureFormatGL = GL_BGR;
        break;
    default:
        cerr << "Error: invalid texture_format_t: " << textureFormat << endl;
        textureFormatGL = GL_RGBA;
    }
    gluBuild2DMipmaps(GL_TEXTURE_2D, bytesPerPixel, width, height, textureFormatGL, GL_UNSIGNED_BYTE, pixels);
//     glTexImage2D(GL_TEXTURE_2D, 0, bytesPerPixel, width, height, 0, textureFormatGL, GL_UNSIGNED_BYTE, pixels);
}

void Renderer::deleteTexture(const size_t textureId) {
    glDeleteTextures(1, &textureId);
}

void Renderer::draw() const {
    if (m_activeCamera->hasChanged()) {
        initCamera();
        m_activeCamera->setHasChanged(false);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float m[16];

    // set camera
    Entity& cam = m_activeCamera->getEntity();
    setOpenGLMatrix(m, VECTOR3_ZERO, cam.getOrientationAbs().inverse());
    glMultMatrixf(m);
    glTranslatef(-cam.getPositionAbs().getX(), -cam.getPositionAbs().getY(), -cam.getPositionAbs().getZ());

    // set lights
    displayLegacyLights();

    // set meshes
    set<RenderableMesh*>::const_iterator it;
    for (it = m_model.begin(); it != m_model.end(); ++it) {
        const Model& model = (*it)->getModel();
        const Entity& entity = (*it)->getEntity();

        glPushMatrix();
        setOpenGLMatrix(m, entity.getPositionAbs(), entity.getOrientationAbs());
        glMultMatrixf(m);
        for (size_t n = 0; n < model.getTotalMeshes(); ++n) {
            const Mesh& mesh = model.getMesh(n);
            const Material& mtl = mesh.getMaterial();

            // set material
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl.getColor(MATERIAL_COLOR_DIFFUSE).rgb);
            glMaterialfv(GL_FRONT, GL_SPECULAR, mtl.getColor(MATERIAL_COLOR_SPECULAR).rgb);
            glMaterialfv(GL_FRONT, GL_AMBIENT, mtl.getColor(MATERIAL_COLOR_AMBIENT).rgb);
            glMaterialfv(GL_FRONT, GL_EMISSION, mtl.getColor(MATERIAL_COLOR_EMISSIVE).rgb);
            glMaterialf(GL_FRONT, GL_SHININESS, mtl.getShininess());

            // set textures
            if (mtl.getTextureMap(MATERIAL_DIFFUSE_MAP) != 0)
                glBindTexture(GL_TEXTURE_2D, mtl.getTextureMap(MATERIAL_DIFFUSE_MAP)->getId());
            else
                glBindTexture(GL_TEXTURE_2D, 0);
            glTexCoordPointer(2, GL_FLOAT, 0, mesh.getUvCoordsPtr());

            // draw mesh
            glVertexPointer(3, GL_FLOAT, 0, mesh.getVerticesPtr());
            glNormalPointer(GL_FLOAT, 0, mesh.getNormalsPtr());
            glDrawElements(GL_TRIANGLES, mesh.getTotalIndices(), GL_UNSIGNED_INT, mesh.getIndicesPtr());
        }
        glPopMatrix();
    }
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

    ss << "Renderer Meshes List:" << endl;
    set<RenderableMesh*>::const_iterator itMesh;
    for (itMesh = m_model.begin(); itMesh != m_model.end(); ++itMesh)
        ss << "  " << (*itMesh)->getDescription() << endl;

    ss << "Lights List:" << endl;
    set<Light*>::const_iterator itLight;
    for (itLight = m_lights.begin(); itLight != m_lights.end(); ++itLight)
        ss << "  " << (*itLight)->getDescription() << endl;

    return ss.str();
}

Renderer::Renderer():
    m_activeCamera(0),
    m_cameras(),
    m_lights(),
    m_model()
{
    GLint integer;

    cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
    cout << "Shader language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
    cout << "Vendor: " << glGetString(GL_VENDOR) << endl;
    cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
    cout << "Using OpenGL Legacy" << endl;
    glGetIntegerv(GL_NUM_EXTENSIONS, &integer);
    cout << integer << " extensions" << endl;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &integer);
    cout << "Max texture size: " << integer << endl;
    //     GLint maxElements = 0;
//     glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &maxElements);
//     cout << "Vertex limit: " << maxElements << endl;
//     glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &maxElements);
//     cout << "Index limit: " << maxElements << endl;
}

Renderer::Renderer(const Renderer& rhs):
    m_activeCamera(rhs.m_activeCamera),
    m_cameras(rhs.m_cameras),
    m_lights(rhs.m_lights),
    m_model(rhs.m_model)
{
    cerr << "Renderer copy constructor should not be called" << endl;
}

Renderer& Renderer::operator=(const Renderer& rhs) {
    cerr << "Renderer assignment operator should not be called" << endl;
    if (this == &rhs)
        return *this;
    m_activeCamera = rhs.m_activeCamera;
    m_cameras = rhs.m_cameras;
    m_lights = rhs.m_lights;
    m_model = rhs.m_model;
    return *this;
}

void Renderer::initialize() {
    // always
    glFrontFace(GL_CCW); // redundant
    glCullFace(GL_BACK); // redundant
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // enable arrays for Vertex Array (legacy)
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Renderer::deinitialize() {
    set<Camera*>::const_iterator itCam;
    for (itCam = m_cameras.begin(); itCam != m_cameras.end(); ++itCam)
        delete *itCam;

    set<Light*>::const_iterator itLight;
    for (itLight = m_lights.begin(); itLight != m_lights.end(); ++itLight)
        delete *itLight;

    set<RenderableMesh*>::const_iterator itMesh;
    for (itMesh = m_model.begin(); itMesh != m_model.end(); ++itMesh)
        delete *itMesh;
}

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

void Renderer::displayLegacyLights() const {
    set<Light*>::const_iterator itLight = m_lights.begin();
    for (size_t i = 0; i < m_lights.size(); ++i) {
        GLenum lightEnum;
        switch (i) {
            case 0:
                lightEnum = GL_LIGHT0;
                break;
            case 1:
                lightEnum = GL_LIGHT1;
                break;
            case 2:
                lightEnum = GL_LIGHT2;
                break;
            case 3:
                lightEnum = GL_LIGHT3;
                break;
            case 4:
                lightEnum = GL_LIGHT4;
                break;
            case 5:
                lightEnum = GL_LIGHT5;
                break;
            case 6:
                lightEnum = GL_LIGHT6;
                break;
            case 7: default:
                lightEnum = GL_LIGHT7;
                break;
        }
        const Vector3& pos = (*itLight)->getEntity().getPositionAbs();
        float lightPosition[] = {float(pos.getX()), float(pos.getY()), float(pos.getZ()), 1.0f};
        glLightfv(lightEnum, GL_POSITION, lightPosition);
        ++itLight;
    }
}

void Renderer::setOpenGLMatrix(float*const m, const Vector3& pos, const Quaternion& rot) const {
    Matrix3x3 temp(rot);
    m[0]  = static_cast<float>(temp.getRow(0).getX());
    m[1]  = static_cast<float>(temp.getRow(1).getX());
    m[2]  = static_cast<float>(temp.getRow(2).getX());
    m[3]  = 0.0f;
    m[4]  = static_cast<float>(temp.getRow(0).getY());
    m[5]  = static_cast<float>(temp.getRow(1).getY());
    m[6]  = static_cast<float>(temp.getRow(2).getY());
    m[7]  = 0.0f;
    m[8]  = static_cast<float>(temp.getRow(0).getZ());
    m[9]  = static_cast<float>(temp.getRow(1).getZ());
    m[10] = static_cast<float>(temp.getRow(2).getZ());
    m[11] = 0.0f;
    m[12] = static_cast<float>(pos.getX());
    m[13] = static_cast<float>(pos.getY());
    m[14] = static_cast<float>(pos.getZ());
    m[15] = 1.0f;
}
