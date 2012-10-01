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
    m_keyPressMap(),
    m_keyReleaseMap(),
    m_keyPressedMap(),
    m_mouseButtonPressMap(),
    m_mouseButtonReleaseMap(),
    m_mouseButtonPressedMap(),
    m_mouseMotionList(),
    m_lastMouseMotion()
{}

void InputManager::bindInput(const input_t type, const string& command, const size_t code) {
    Command cmd(0, 0, "");
    if (cmd.parseCommand(command)) {
        pair<size_t, Command> binding(code, cmd);
        switch (type) {
        case INPUT_KEY_PRESS:
            m_keyPressMap.insert(binding);
            break;
        case INPUT_KEY_RELEASE:
            m_keyReleaseMap.insert(binding);
            break;
        case INPUT_KEY_PRESSED:
            m_keyPressedMap.insert(binding);
            break;
        case INPUT_MOUSE_BUTTON_PRESS:
            m_mouseButtonPressMap.insert(binding);
            break;
        case INPUT_MOUSE_BUTTON_RELEASE:
            m_mouseButtonReleaseMap.insert(binding);
            break;
        case INPUT_MOUSE_BUTTON_PRESSED:
            m_mouseButtonPressedMap.insert(binding);
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
    m_keyPressMap.clear();
    m_keyReleaseMap.clear();
    m_keyPressedMap.clear();
    m_mouseButtonPressMap.clear();
    m_mouseButtonReleaseMap.clear();
    m_mouseButtonPressedMap.clear();
    m_mouseMotionList.clear();
}

void InputManager::onKeyPress(const size_t code) {
    input_map_t::iterator it = m_keyPressMap.find(code);
    if (it != m_keyPressMap.end())
        Terminal::pushCommand(it->second);
}

void InputManager::onKeyRelease(const size_t code) {
    input_map_t::iterator it = m_keyReleaseMap.find(code);
    if (it != m_keyReleaseMap.end())
        Terminal::pushCommand(it->second);
}

void InputManager::onKeyPressed(const size_t code) {
    input_map_t::iterator it = m_keyPressedMap.find(code);
    if (it != m_keyPressedMap.end())
        Terminal::pushCommand(it->second);
}

void InputManager::onMouseButtonPress(const size_t code) {
    input_map_t::iterator it = m_mouseButtonPressMap.find(code);
    if (it != m_mouseButtonPressMap.end())
        Terminal::pushCommand(it->second);
}

void InputManager::onMouseButtonRelease(const size_t code) {
    input_map_t::iterator it = m_mouseButtonReleaseMap.find(code);
    if (it != m_mouseButtonReleaseMap.end())
        Terminal::pushCommand(it->second);
}

void InputManager::onMouseButtonPressed(const size_t code) {
    input_map_t::iterator it = m_mouseButtonPressedMap.find(code);
    if (it != m_mouseButtonPressedMap.end())
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

    out << "Key Press Map:" << endl;
    for (it = rhs.m_keyPressMap.begin(); it != rhs.m_keyPressMap.end(); ++it)
        out << "\t" << it->first << "\t" << it->second << endl;

    out << "Key Release Map:" << endl;
    for (it = rhs.m_keyReleaseMap.begin(); it != rhs.m_keyReleaseMap.end(); ++it)
        out << "\t" << it->first << "\t" << it->second << endl;

    out << "Key Pressed Map:" << endl;
    for (it = rhs.m_keyPressedMap.begin(); it != rhs.m_keyPressedMap.end(); ++it)
        out << "\t" << it->first << "\t" << it->second << endl;

    out << "Mouse Button Press Map:" << endl;
    for (it = rhs.m_mouseButtonPressMap.begin(); it != rhs.m_mouseButtonPressMap.end(); ++it)
        out << "\t" << it->first << "\t" << it->second << endl;

    out << "Mouse Button Release Map:" << endl;
    for (it = rhs.m_mouseButtonReleaseMap.begin(); it != rhs.m_mouseButtonReleaseMap.end(); ++it)
        out << "\t" << it->first << "\t" << it->second << endl;

    out << "Mouse Button Pressed Map:" << endl;
    for (it = rhs.m_mouseButtonPressedMap.begin(); it != rhs.m_mouseButtonPressedMap.end(); ++it)
        out << "\t" << it->first << "\t" << it->second << endl;

    out << "Mouse Motion Map:" << endl;
    for (size_t i = 0; i < rhs.m_mouseMotionList.size(); ++i)
        out << "\t" << rhs.m_mouseMotionList[i] << endl;

    return out;
}
