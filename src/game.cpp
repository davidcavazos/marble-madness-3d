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


#include "game.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <SDL.h>
#include "engine/kernel/device.hpp"
#include "engine/kernel/devicemanager.hpp"
#include "engine/kernel/terminal.hpp"
#include "engine/kernel/scenemanager.hpp"
#include "engine/renderer/rendermanager.hpp"
#include "engine/renderer/renderer.hpp"
#include "engine/renderer/renderablemesh.hpp"
#include "engine/renderer/camera.hpp"
#include "engine/renderer/light.hpp"
#include "engine/resources/resourcemanager.hpp"
#include "engine/physics/rigidbody.hpp"
#include "engine/physics/physicsworld.hpp"
#include "engine/physics/physicsmanager.hpp"

using namespace std;

const double FIRE_SPEED = 50.0;
const double MISSILE_SIZE = 0.5;


Game::Game(const string& objectName, const string& rootNodeName):
    CommandObject(objectName),
    m_isRunning(false),
    m_sceneManager(rootNodeName)
{
    registerCommand("quit", boost::bind(&Game::quit, this, _1));
    registerCommand("run", boost::bind(&Game::runCommand, this, _1));
    registerCommand("print-entity", boost::bind(&Game::printEntity, this, _1));
    registerCommand("on-mouse-motion", boost::bind(&Game::onMouseMotion, this, _1));
    registerCommand("fire-cube", boost::bind(&Game::fireCube, this, _1));
    registerCommand("fire-sphere", boost::bind(&Game::fireSphere, this, _1));

    Device* device = DeviceManager::create();
    device->setTitle("Marble Madness 3D");
    device->setResolution(800, 500);
//     device->setFullscreen();

    ResourceManager::create();
    RenderManager::create();
    PhysicsManager::create();
}

Game::~Game() {
    PhysicsManager::shutdown();
    RenderManager::shutdown();
    ResourceManager::shutdown();
    DeviceManager::shutdown();
}

void Game::loadScene() {
    cout << "Loading scene..." << endl;

    Entity* root = m_sceneManager.getRootPtr();

    Entity* floor = root->addChild("floor");
    floor->setPositionAbs(0.0f, -1.0f, 0.0f);
    RenderableMesh* floorMesh = new RenderableMesh(floor);
    floorMesh->loadBox(100, 1, 100);
    RigidBody* floorBody = new RigidBody(floor);
    floorBody->addBox(100, 1, 100);

    Entity* b1 = root->addChild("b1");
    b1->setPositionAbs(5.0f, 4.0f, -10.0f);
    RenderableMesh* b1Mesh = new RenderableMesh(b1);
    b1Mesh->loadBox(3.0f, 9.0f, 3.0f);
    RigidBody* b1Body = new RigidBody(b1);
    b1Body->addBox(3, 9, 3);

    // model            faces (triangles)
    // icosphere1              20
    // icosphere2              80
    // icosphere3             320
    // icosphere4           1,280
    // icosphere5           5,120
    // icosphere6          20,480
    // icosphere7          81,920
    // icosphere8         327,680
    // icosphere9       1,310,720
    // icosphere10      5,242,880
    Entity* cube = root->addChild("cube");
    cube->setPositionAbs(-1.0, 8.0, 0.0);
    cube->setOrientationAbs(0.5, 0.3, 0.2);
    RenderableMesh* cubeMesh = new RenderableMesh(cube);
    cubeMesh->loadBox(0.5, 0.5, 0.5);
    RigidBody* cubeBody = new RigidBody(cube);
    cubeBody->init(1.0);
    cubeBody->addBox(0.5, 0.5, 0.5);

    Entity* mesh = root->addChild("model");
    mesh->setPositionRel(1.5f, 5.0f, 0.0f);
    RenderableMesh* renderableMesh = new RenderableMesh(mesh);
    renderableMesh->loadFromFile("assets/meshes/materialtest.dae");
    RigidBody* meshBody = new RigidBody(mesh);
    meshBody->init(10.0);
    meshBody->addBox(2, 2, 2);
//     meshBody->addConvexHull("assets/meshes/materialtest.dae");

    Entity* camera = root->addChild("camera");
    camera->setPositionAbs(0.0f, 4.0f, 10.0f);
    camera->pitch(-0.2);
//     camera->lookAt(cube->getPositionAbs(), VECTOR3_UNIT_Y);
    Camera* camComponent = new Camera(camera, CAMERA_PROJECTION);
    camComponent->setPerspectiveFOV(45.0);
//     RigidBody* cameraBody = new RigidBody(camera);
//     cameraBody->addSphere(1);

    Entity* light1 = root->addChild("light1");
    light1->setPositionAbs(5, 5, 0);
    Light* light1Cmp = new Light(light1);
    light1Cmp->setDiffuse(1.0, 1.0, 1.0);

//     Entity* light2 = root->addChild("light2");
//     light2->setPositionAbs(0, 5, 5);
//     Light* light2Cmp = new Light(light2);
//     light2Cmp->setDiffuse(0.0, 1.0, 0.0);

//     Entity* light3 = root->addChild("light3");
//     light3->setPositionAbs(0, 5, 0);
//     Light* light3Cmp = new Light(light3);
//     light3Cmp->setDiffuse(0.0, 0.0, 1.0);

//     cout << Terminal::listsToString() << endl;
//     cout << m_sceneManager.sceneGraphToString() << endl;
//     cout << RenderManager::getRenderer().listsToString() << endl;
//     cout << ResourceManager::listsToString() << endl;
}

void Game::bindInputs() {
    cout << "Binding inputs..." << endl;
    InputManager& inputs = DeviceManager::getDevice().getInputManager();
    inputs.bindInput(INPUT_KEY_RELEASE, "game quit", SDLK_ESCAPE);
    inputs.bindInput(INPUT_KEY_RELEASE, "game run commands.txt", SDLK_TAB);
    inputs.bindInput(INPUT_MOUSE_MOTION, "game on-mouse-motion");

    inputs.bindInput(INPUT_KEY_PRESSED, "camera move-z        -5", SDLK_w);
    inputs.bindInput(INPUT_KEY_PRESSED, "camera move-x        -5", SDLK_a);
    inputs.bindInput(INPUT_KEY_PRESSED, "camera move-z         5", SDLK_s);
    inputs.bindInput(INPUT_KEY_PRESSED, "camera move-x         5", SDLK_d);
    inputs.bindInput(INPUT_KEY_PRESSED, "camera move-y-global  5", SDLK_SPACE);
    inputs.bindInput(INPUT_KEY_PRESSED, "camera move-y-global -5", SDLK_LSHIFT);
    inputs.bindInput(INPUT_MOUSE_BUTTON_RELEASE, "game fire-cube", 1);
    inputs.bindInput(INPUT_MOUSE_BUTTON_RELEASE, "game fire-sphere", 3);
}

void Game::runGameLoop() {
    Uint32 startTime;
    Uint32 deltaTime;

    Device* device = DeviceManager::getDevicePtr();
    Renderer* renderer = RenderManager::getRendererPtr();
    PhysicsWorld* world = PhysicsManager::getPhysicsWorldPtr();
    renderer->initLighting();
    renderer->setAmbientLight(0.2f, 0.2f, 0.2f);

    cout << "Entering game loop" << endl;
    m_isRunning = true;
    while (m_isRunning) {
        startTime = SDL_GetTicks();
        device->onFrameStart();

        world->stepSimulation(0.001 * SDL_GetTicks());

        device->processEvents(m_isRunning);
        Terminal::processCommandsQueue();

        stringstream ss;
        deltaTime = SDL_GetTicks() - startTime;
        ss << "Marble Madness 3D - CPU:" << setw(3) << deltaTime << " ms - ";

        startTime = SDL_GetTicks();
        renderer->draw();
        device->swapBuffers();
        deltaTime = SDL_GetTicks() - startTime;
        ss << "GPU:" << setw(3) << deltaTime << " ms (16-40 ideal) - ";

        device->onFrameEnd();
        ss << setw(5) << fixed << setprecision(1) << device->getFps() << " fps";
        device->setTitle(ss.str());
    }
}

void Game::quit(const string&) {
    m_isRunning = false;
}

void Game::runCommand(const string& arg) {
    string cmd;

    fstream file(arg.c_str(), ios::in);
    while (file.good()) {
        getline(file, cmd);
        if (!cmd.empty()) {
            cout << "> " << cmd << endl;
            Terminal::pushCommand(cmd);
        }
    }
    file.close();
}

void Game::printEntity(const string& arg) {
    Entity* entity;
    if (m_sceneManager.findEntity(arg, entity))
        cout << *entity << endl;
}

void Game::onMouseMotion(const string&) {
    static Command moveXCmd("camera yaw-global");
    static Command moveYCmd("camera pitch");

    mouse_motion_t motion = DeviceManager::getDevice().getInputManager().getLastMouseMotion();

    float sensitivity = 0.05;
    stringstream ssx;
    ssx << motion.xrel * sensitivity;
    moveXCmd.setArguments(ssx.str());
    Terminal::pushCommand(moveXCmd);

    stringstream ssy;
    ssy << motion.yrel * sensitivity;
    moveYCmd.setArguments(ssy.str());
    Terminal::pushCommand(moveYCmd);
}

void Game::fireCube(const std::string&) {
    Entity* camera;
    if (m_sceneManager.findEntity("camera", camera)) {
        static size_t n = 0;
        stringstream ss;
        ss << "missile-cube-" << ++n;

        Entity* cube = m_sceneManager.getRoot().addChild(ss.str());
        Vector3 orientationUnit = VECTOR3_UNIT_Z_NEG.rotate(camera->getOrientationAbs());
        cube->setPositionAbs(camera->getPositionAbs() + orientationUnit);
        cube->setOrientationAbs(camera->getOrientationAbs());

        RenderableMesh* cubeMesh = new RenderableMesh(cube);
        cubeMesh->loadBox(MISSILE_SIZE, MISSILE_SIZE, MISSILE_SIZE);

        RigidBody* cubeBody = new RigidBody(cube);
        cubeBody->init(1.0, 0.8);
        cubeBody->addBox(MISSILE_SIZE, MISSILE_SIZE, MISSILE_SIZE);
        cubeBody->setLinearVelocity(orientationUnit * FIRE_SPEED);
    }
}

void Game::fireSphere(const std::string&) {
    Entity* camera;
    if (m_sceneManager.findEntity("camera", camera)) {
        static size_t n = 0;
        stringstream ss;
        ss << "missile-sphere-" << ++n;

        Entity* sphere = m_sceneManager.getRoot().addChild(ss.str());
        Vector3 orientationUnit = VECTOR3_UNIT_Z_NEG.rotate(camera->getOrientationAbs());
        sphere->setPositionAbs(camera->getPositionAbs() + orientationUnit);
        sphere->setOrientationAbs(camera->getOrientationAbs());

        RenderableMesh* cubeMesh = new RenderableMesh(sphere);
        cubeMesh->loadFromFile("assets/meshes/icosphere3.dae");

        RigidBody* cubeBody = new RigidBody(sphere);
        cubeBody->init(1.0, 0.8);
        cubeBody->addSphere(1.0);
//         cubeBody->addConvexHull("assets/meshes/icosphere1.dae");
        cubeBody->setLinearVelocity(orientationUnit * FIRE_SPEED);
    }
}
