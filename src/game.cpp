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
#include "engine/device/devicemanager.hpp"
#include "engine/terminal/terminal.hpp"

using namespace std;

Game::Game(const string& objectName):
    CommandObject(objectName),
    m_isRunning(false)
{
    registerCommands();
    Device* device = DeviceManager::createSystem(DEVICE_SDL_OPENGL_LEGACY);
    device->setTitle("Marble Madness 3D");
    device->setResolution(800, 500);
}

Game::~Game() {
    DeviceManager::shutdownSystem();
}

void Game::loadScene() {
    cout << "Loading scene" << endl;
}

void Game::bindControls() {
    cout << "Binding controls" << endl;
}

void Game::runGameLoop() {
    cout << "Entering game loop" << endl;
    Device* device = DeviceManager::getDevice();
    m_isRunning = true;
    while (m_isRunning) {
        string cmd;
        cout << "terminal# ";
        getline(cin, cmd);
        Terminal::pushCommand(cmd);

        device->processEvents(m_isRunning);
        Terminal::processCommandsQueue();
    }
}

void Game::registerCommands() {
    registerCommand("quit", boost::bind(&Game::quit, this, _1));
}

void Game::quit(const string&) {
    m_isRunning = false;
}
