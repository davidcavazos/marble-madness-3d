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


#include "engine/kernel/inputmanager.hpp"

#include <iostream>
#include <sstream>
#include "engine/kernel/terminal.hpp"

using namespace std;

InputManager::InputManager():
    m_keyDownMap(),
    m_keyUpMap(),
    m_mouseButtonDownMap(),
    m_mouseButtonUpMap(),
    m_mouseMotionList(),
    m_lastMouseMotion()
{}

void InputManager::bindInput(const input_t type, const string& command, const size_t code) {
    Command cmd(0, 0, "");
    if (cmd.parseCommand(command)) {
        pair<size_t, Command> binding(code, cmd);
        switch (type) {
        case INPUT_KEY_DOWN:
            m_keyDownMap.insert(binding);
            break;
        case INPUT_KEY_UP:
            m_keyUpMap.insert(binding);
            break;
        case INPUT_MOUSE_BUTTON_DOWN:
            m_mouseButtonDownMap.insert(binding);
            break;
        case INPUT_MOUSE_BUTTON_UP:
            m_mouseButtonUpMap.insert(binding);
            break;
        case INPUT_MOUSE_MOTION:
            m_mouseMotionList.push_back(cmd);
            break;
        default:
            cerr << "Invalid input_t: " << type << endl;
        }
    }
}

void InputManager::clearAllBindings() {
    m_keyDownMap.clear();
    m_keyUpMap.clear();
    m_mouseButtonDownMap.clear();
    m_mouseButtonUpMap.clear();
    m_mouseMotionList.clear();
}

void InputManager::onKeyDown(const size_t code) {
    input_map_t::iterator it = m_keyDownMap.find(code);
    if (it != m_keyDownMap.end())
        Terminal::pushCommand(it->second);
}

void InputManager::onKeyUp(const size_t code) {
    input_map_t::iterator it = m_keyUpMap.find(code);
    if (it != m_keyUpMap.end())
        Terminal::pushCommand(it->second);
}

void InputManager::onMouseButtonDown(const size_t code) {
    input_map_t::iterator it = m_mouseButtonDownMap.find(code);
    if (it != m_mouseButtonDownMap.end())
        Terminal::pushCommand(it->second);
}

void InputManager::onMouseButtonUp(const size_t code) {
    input_map_t::iterator it = m_mouseButtonUpMap.find(code);
    if (it != m_mouseButtonUpMap.end())
        Terminal::pushCommand(it->second);
}

void InputManager::onMouseMotion(const mouse_motion_t& motion) {
    m_lastMouseMotion = motion;
    for (size_t i = 0; i < m_mouseMotionList.size(); ++i) {
//         stringstream ss;
//         ss << x << " " << y << " " << xrel << " " << yrel;
//         m_mouseMotionList[i].setArguments(ss.str());
        Terminal::pushCommand(m_mouseMotionList[i]);
    }
}

ostream& operator<<(ostream& out, const InputManager& rhs) {
    map<size_t, Command>::const_iterator it;

    out << "Key Down Map:" << endl;
    for (it = rhs.m_keyDownMap.begin(); it != rhs.m_keyDownMap.end(); ++it)
        out << "\t" << it->first << "\t" << it->second << endl;

    out << "Key Up Map:" << endl;
    for (it = rhs.m_keyUpMap.begin(); it != rhs.m_keyUpMap.end(); ++it)
        out << "\t" << it->first << "\t" << it->second << endl;

    out << "Mouse Button Down Map:" << endl;
    for (it = rhs.m_mouseButtonDownMap.begin(); it != rhs.m_mouseButtonDownMap.end(); ++it)
        out << "\t" << it->first << "\t" << it->second << endl;

    out << "Mouse Button Up Map:" << endl;
    for (it = rhs.m_mouseButtonUpMap.begin(); it != rhs.m_mouseButtonUpMap.end(); ++it)
        out << "\t" << it->first << "\t" << it->second << endl;

    out << "Mouse Motion Map:" << endl;
    for (size_t i = 0; i < rhs.m_mouseMotionList.size(); ++i)
        out << "\t" << rhs.m_mouseMotionList[i] << endl;

    return out;
}
