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


#include "engine/kernel/scenemanager.hpp"

using namespace std;

map<string, Entity*> SceneManager::ms_entities = map<string, Entity*>();

SceneManager::SceneManager(const string& rootNodeName):
    m_root(0, rootNodeName)
{}

bool SceneManager::findEntity(const string& name, Entity*& entity) {
    map<string, Entity*>::iterator it = ms_entities.find(name);
    if (it != ms_entities.end()) {
        entity = it->second;
        return true;
    }
    return false;
}

string SceneManager::sceneGraphToString() {
    stringstream ss;
    ss << "Scene Graph:" << endl;
    ss << m_root.treeToString(0);
    return ss.str();
}
