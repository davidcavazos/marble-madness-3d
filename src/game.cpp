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
#include <SDL_events.h>
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

    Camera* camComponent = new Camera(CAMERA_PROJECTION);
    Entity* camera = root->addChild("camera");
    camera->attachComponent(camComponent);
//     camera->setPosition(0.0, 0.0, -5.0);

    RenderableMesh* mesh = new RenderableMesh;
    mesh->generateCube(1.0, 1.0, 1.0);
    Entity* cube = root->addChild("cube");
    cube->attachComponent(mesh);
    cube->setPosition(0.0, 0.0, 10.0);

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
    device->getInputManager().bindInput(INPUT_KEY_UP, "game quit", SDLK_ESCAPE);
    device->getInputManager().bindInput(INPUT_KEY_UP, "game run commands.txt", SDLK_SPACE);
}

void Game::runGameLoop() {
    cout << "Creating renderer" << endl;
    Renderer* renderer = RenderManager::create();
    cout << "Entering game loop" << endl;
    Device* device = DeviceManager::getDevicePtr();
    m_isRunning = true;
    while (m_isRunning) {
        device->processEvents(m_isRunning);
        RenderManager::renderFrame();
        device->swapBuffers();
        Terminal::processCommandsQueue();
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
