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


#ifndef RENDERMANAGER_HPP
#define RENDERMANAGER_HPP

#include <string>
#include <set>

class Renderer;
class Camera;
class RenderableMesh;

class RenderManager {
public:
    friend class Renderer;
    friend class Camera;
    friend class RenderableMesh;

    static Renderer* getRendererPtr();
    static Renderer& getRenderer();

    static Renderer* create();
    static void shutdown();
    static std::string listsToString();

private:
    static Renderer* ms_renderer;
    static Camera* ms_activeCamera;
    static std::set<Camera*> ms_cameras;
    static std::set<RenderableMesh*> ms_meshes;
};



inline Renderer* RenderManager::getRendererPtr() {
    return ms_renderer;
}

inline Renderer& RenderManager::getRenderer() {
    return *getRendererPtr();
}

#endif // RENDERMANAGER_HPP
