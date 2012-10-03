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

using namespace std;

Game::Game(const string& objectName, const string& rootNodeName):
    CommandObject(objectName),
    m_isRunning(false),
    m_sceneManager(rootNodeName)
{
    cout << "TODO:" << endl;
    cout << "+ Recognize first character # as comment in terminal" << endl;
    cout << "+ Make input manager into a vector, only one can be active at a time (XML)" << endl;
    cout << "+ Read/write scene from XML" << endl;
    cout << "+ Possibility of multiple cameras" << endl;
    cout << "+ Binding keys (or other inputs) with a command" << endl;
    cout << "+ Implement OpenGL Core" << endl;
    cout << endl;

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

    Entity* camera = root->addChild("camera");
    Camera* camComponent = new Camera(camera, CAMERA_PROJECTION);
    camComponent->setPerspectiveFOV(45.0);
    camera->getTransform().setPosition(0.0, 0.0, 5.0);

    Entity* cube = root->addChild("cube");
    cube->getTransform().setPosition(0.0, 0.0, 0.0);
    cube->getTransform().setRotation(0.0f, 0.0f, 0.0f);
    RenderableMesh* mesh = new RenderableMesh(cube);
    mesh->generateCube(1.0, 1.0, 1.0);

    Entity* enemy1 = root->addChild("enemy1");
    Entity* gun1 = enemy1->addChild("gun1");
    gun1->addChild("scope");
    Entity* enemy2 = root->addChild("enemy2");
    enemy2->addChild("gun2");
    root->addChild("enemy3");

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

    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "camera yaw 0.005", SDLK_RIGHT);
    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "camera yaw -0.005", SDLK_LEFT);
    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "camera pitch 0.005", SDLK_DOWN);
    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "camera pitch -0.005", SDLK_UP);
    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "camera roll 0.05", SDLK_q);
    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "camera roll -0.05", SDLK_e);

    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "camera move-z 2", SDLK_s);
    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "camera move-z -2", SDLK_w);
    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "camera move-x 2", SDLK_d);
    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "camera move-x -2", SDLK_a);
    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "camera move-y 2", SDLK_SPACE);
    device->getInputManager().bindInput(INPUT_KEY_PRESSED, "camera move-y -2", SDLK_LSHIFT);
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
    static Command moveXCmd("camera yaw");
    static Command moveYCmd("camera pitch");

    mouse_motion_t motion = DeviceManager::getDevice().getInputManager().getLastMouseMotion();

    float sensitivity = 0.05;
    stringstream ssx;
    ssx << -motion.xrel * sensitivity * DeviceManager::getDeltaTime();
//     ssx << " " << -motion.yrel * sensitivity * DeviceManager::getDeltaTime();
    moveXCmd.setArguments(ssx.str());
    Terminal::pushCommand(moveXCmd);

    stringstream ssy;
    ssy << -motion.yrel * sensitivity * DeviceManager::getDeltaTime();
    moveYCmd.setArguments(ssy.str());
    Terminal::pushCommand(moveYCmd);
}
