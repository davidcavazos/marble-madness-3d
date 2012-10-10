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
#include "engine/kernel/common.hpp"
#include "engine/renderer/rendermanager.hpp"
#include "engine/renderer/renderer.hpp"
#include "engine/renderer/renderablemesh.hpp"
#include "engine/renderer/camera.hpp"

using namespace std;

Game::Game(const string& objectName, const string& rootNodeName):
    CommandObject(objectName),
    m_isRunning(false),
    m_sceneManager(rootNodeName)
{
    cout << "TODO:" << endl;
    cout << "+ Transform.lookAt(vector3) <OgreCamera.cpp>" << endl;
    cout << "+ Load meshes from .collada (maybe later make a conversion tool to binary)" << endl;
    cout << "+ Add more primitives (sphere, cilinder, etc) and cube divisions" << endl;
    cout << "+ Detect OpenGL version and capabilities" << endl;
    cout << "+ Implement OpenGL Core and shaders" << endl;
    cout << "+ Physics!!" << endl;
    cout << "~ Save input commands as a replay script" << endl;
    cout << "~ Read/write scene from XML" << endl;
    cout << "- Make input manager into a vector, only one can be active at a time (XML)" << endl;
    cout << "- Binding keys (or other inputs) with a command" << endl;
    cout << "- Possibility of multiple cameras" << endl;
    cout << endl;

    /*
     * Renderer features
     *  stencil shadows
     *  deferred shading
     */

    registerCommand("quit", boost::bind(&Game::quit, this, _1));
    registerCommand("run", boost::bind(&Game::runCommand, this, _1));
    registerCommand("print-entity", boost::bind(&Game::printEntity, this, _1));
    registerCommand("on-mouse-motion", boost::bind(&Game::onMouseMotion, this, _1));

    Device* device = DeviceManager::create();
    device->setTitle("Marble Madness 3D");
    device->setResolution(800, 500);
}

Game::~Game() {
    RenderManager::shutdown();
    DeviceManager::shutdown();
}

void Game::loadScene() {
    cout << "Loading scene" << endl;
    Entity* root = m_sceneManager.getRootPtr();

    Entity* floor = root->addChild("floor");
    floor->setPositionAbs(0.0f, -1.0f, 0.0f);
    RenderableMesh* floorMesh = new RenderableMesh(floor);
    floorMesh->generateCube(100.0f, 0.1f, 100.0f);

    Entity* b1 = root->addChild("b1");
    b1->setPositionAbs(5.0f, 0.0f, -10.0f);
    RenderableMesh* b1Mesh = new RenderableMesh(b1);
    b1Mesh->generateCube(3.0f, 13.0f, 3.0f);

    Entity* camera = root->addChild("camera");
    camera->setPositionAbs(0.0f, 1.0f, 5.0f);
//     camera->getTransform().lookAt(cube2->getTransform().getPosition());
    Camera* camComponent = new Camera(camera, CAMERA_PROJECTION);
    camComponent->setPerspectiveFOV(50.0);

    Entity* cube = root->addChild("cube");
    cube->setPositionAbs(0.0f, 0.0f, 0.0f);
    cube->setOrientationAbs(0.2f, 0.2f, 0.1f);
    RenderableMesh* mesh = new RenderableMesh(cube);
    mesh->generateCube(1.0f, 0.5f, 1.5f);

    Entity* cube2 = cube->addChild("cube2");
    cube2->setPositionRel(2.0f, 0.5f, 0.0f);
    RenderableMesh* mesh2 = new RenderableMesh(cube2);
    mesh2->generateCube(0.5f, 0.25f, 0.75f);

    cout << Terminal::listsToString() << endl;

    cout << m_sceneManager.sceneGraphToString() << endl;

    cout << RenderManager::listsToString() << endl;
}

void Game::bindControls() {
    cout << "Binding controls" << endl;
    Device* device = DeviceManager::getDevicePtr();
    device->getInputManager().bindInput(INPUT_KEY_RELEASE, "game quit", SDLK_ESCAPE);
    device->getInputManager().bindInput(INPUT_KEY_RELEASE, "game run commands.txt", SDLK_TAB);
    device->getInputManager().bindInput(INPUT_MOUSE_MOTION, "game on-mouse-motion");

    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "cube yaw 1", SDLK_l);
    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "cube yaw -1", SDLK_j);
    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "cube pitch 1", SDLK_k);
    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "cube pitch -1", SDLK_i);
    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "cube roll 1", SDLK_u);
    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "cube roll -1", SDLK_o);
//     device->getInputManager().bindInput(INPUT_KEY_PRESSED, "cube2 yaw-parent 1", SDLK_RIGHT);
//     device->getInputManager().bindInput(INPUT_KEY_PRESSED, "cube2 yaw-parent -1", SDLK_LEFT);
//     device->getInputManager().bindInput(INPUT_KEY_PRESSED, "cube2 pitch-parent 1", SDLK_DOWN);
//     device->getInputManager().bindInput(INPUT_KEY_PRESSED, "cube2 pitch-parent -1", SDLK_UP);
    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "cube move-z -2", SDLK_UP);
    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "cube move-x -2", SDLK_LEFT);
    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "cube move-z 2", SDLK_DOWN);
    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "cube move-x 2", SDLK_RIGHT);

    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "camera move-z -2", SDLK_w);
    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "camera move-x -2", SDLK_a);
    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "camera move-z 2", SDLK_s);
    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "camera move-x 2", SDLK_d);
    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "camera move-y-global 2", SDLK_SPACE);
    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "camera move-y-global -2", SDLK_LSHIFT);
}

void Game::runGameLoop() {
    Uint32 startTime;
    Uint32 deltaTime;

    Device* device = DeviceManager::getDevicePtr();
//     device->trapCursor();
    device->hideCursor();
    cout << "Creating renderer" << endl;
    Renderer* renderer = RenderManager::create();
    cout << "Entering game loop" << endl;

    int screenCenterX = device->getWinWidth() / 2;
    int screenCenterY = device->getWinHeight() / 2;
    m_isRunning = true;
    while (m_isRunning) {
        device->setCursorPos(screenCenterX, screenCenterY);
        startTime = SDL_GetTicks();
        device->onFrameStart();

        device->processEvents(m_isRunning);
        Terminal::processCommandsQueue();

        deltaTime = SDL_GetTicks() - startTime;
        stringstream ss;
        ss << "Marble Madness 3D - " << setw(3) << deltaTime << " ms (16-40 ideal) - ";

        renderer->draw();
        device->swapBuffers();

        device->onFrameEnd();
        ss << fixed << setprecision(1) << device->getFps() << " fps";
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
//     ssx << " " << -motion.yrel * sensitivity;
    moveXCmd.setArguments(ssx.str());
    Terminal::pushCommand(moveXCmd);

    stringstream ssy;
    ssy << motion.yrel * sensitivity;
    moveYCmd.setArguments(ssy.str());
    Terminal::pushCommand(moveYCmd);
}
