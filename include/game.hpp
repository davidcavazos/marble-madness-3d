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


#ifndef GAME_HPP
#define GAME_HPP

#include "engine/kernel/scenemanager.hpp"

class Device;

class Game: public CommandObject {
public:
    Game(const std::string& objectName, const std::string& rootNodeName);
    ~Game();

    void loadScene();
    void bindControls();
    void runGameLoop();

private:
    bool m_isRunning;
    SceneManager m_sceneManager;

    void quit(const std::string&);
    void runCommand(const std::string& arg);
    void printEntity(const std::string& arg);
};

#endif // GAME_HPP
