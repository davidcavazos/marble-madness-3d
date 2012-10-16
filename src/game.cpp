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

    ResourceManager::create();

    Device* device = DeviceManager::create();
    device->setTitle("Marble Madness 3D");
    device->setResolution(800, 500);

//     PhysicsWorld* world =
    PhysicsManager::create();
}

Game::~Game() {
    RenderManager::shutdown();
    PhysicsManager::shutdown();
    DeviceManager::shutdown();
    ResourceManager::shutdown();
}

void Game::loadScene() {
    cout << "Loading scene..." << endl;
    Entity* root = m_sceneManager.getRootPtr();

    Entity* floor = root->addChild("floor");
    floor->setPositionAbs(0.0f, -1.0f, 0.0f);
    RenderableMesh* floorMesh = new RenderableMesh(floor);
    floorMesh->loadBox(100, 0.1, 100);
    RigidBody* floorBody = new RigidBody(floor);
    floorBody->addBox(100, 0.1, 100);

    Entity* b1 = root->addChild("b1");
    b1->setPositionAbs(5.0f, 0.0f, -10.0f);
    RenderableMesh* b1Mesh = new RenderableMesh(b1);
    b1Mesh->loadBox(3.0f, 13.0f, 3.0f);
    RigidBody* b1Body = new RigidBody(b1);
    b1Body->addBox(3, 13, 3);

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
    cube->setPositionAbs(1.0, 8.0, 0.0);
    cube->setOrientationAbs(0.5, 0.3, 0.2);
    RenderableMesh* cubeMesh = new RenderableMesh(cube);
    cubeMesh->loadBox(0.5, 0.5, 0.5);
    RigidBody* cubeBody = new RigidBody(cube);
    cubeBody->init(1.0);
    cubeBody->addBox(0.5, 0.5, 0.5);

    Entity* sphere = root->addChild("sphere");
    sphere->setPositionRel(1.5f, 5.0f, 0.0f);
    RenderableMesh* sphereMesh = new RenderableMesh(sphere);
    sphereMesh->loadFromFile("assets/meshes/icosphere5.dae");
    RigidBody* sphereBody = new RigidBody(sphere);
    sphereBody->init(2.0);
    sphereBody->addSphere(1);

    Entity* camera = root->addChild("camera");
    camera->setPositionAbs(0.0f, 4.0f, 10.0f);
    camera->pitch(-0.2);
//     camera->lookAt(cube->getPositionAbs(), VECTOR3_UNIT_Y);
    Camera* camComponent = new Camera(camera, CAMERA_PROJECTION);
    camComponent->setPerspectiveFOV(45.0);

//     cout << Terminal::listsToString() << endl;
//     cout << m_sceneManager.sceneGraphToString() << endl;
//     cout << RenderManager::listsToString() << endl;
//     cout << ResourceManager::listsToString() << endl;
}

void Game::bindControls() {
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
//     device->trapCursor();
    device->hideCursor();

    cout << "Creating renderer..." << endl;
    Renderer* renderer = RenderManager::create();
    cout << "Entering game loop" << endl;

    PhysicsWorld* world = PhysicsManager::getPhysicsWorldPtr();

    int screenCenterX = device->getWinWidth() / 2;
    int screenCenterY = device->getWinHeight() / 2;
    m_isRunning = true;
    while (m_isRunning) {
        device->setCursorPos(screenCenterX, screenCenterY);
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
        Entity* cube = m_sceneManager.getRoot().addChild("missile");
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
        Entity* sphere = m_sceneManager.getRoot().addChild("missile");
        Vector3 orientationUnit = VECTOR3_UNIT_Z_NEG.rotate(camera->getOrientationAbs());
        sphere->setPositionAbs(camera->getPositionAbs() + orientationUnit);
        sphere->setOrientationAbs(camera->getOrientationAbs());

        RenderableMesh* cubeMesh = new RenderableMesh(sphere);
        cubeMesh->loadFromFile("assets/meshes/icosphere1.dae");

        RigidBody* cubeBody = new RigidBody(sphere);
        cubeBody->init(1.0, 0.8);
        cubeBody->addSphere(MISSILE_SIZE);
        cubeBody->setLinearVelocity(orientationUnit * FIRE_SPEED);
    }
}
