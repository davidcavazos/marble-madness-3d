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


#include "engine/resources/texture.hpp"

#include <iostream>
#include <SDL/SDL_image.h>

using namespace std;

Texture::Texture(const std::string& identifier):
    m_identifier(identifier),
    m_width(0),
    m_height(0)
{}

void Texture::load() {
    // http://content.gpwiki.org/index.php/SDL:Tutorials:Using_SDL_with_OpenGL
    SDL_Surface* img = IMG_Load(m_identifier.c_str());
    if (img == 0) {
        cerr << "Error opening image file: " << m_identifier << endl;
        return;
    }

    // check width and height
    if ((img->w & (img->w - 1)) != 0)
        cerr << "Warning: image's width is not a power of 2: " << m_identifier << endl;
    if ((img->h & (img->h - 1)) != 0)
        cerr << "Warning: image's height is not a power of 2: " << m_identifier << endl;
    m_width = img->w;
    m_height = img->h;

    // check for number of channels in each pixel
    switch (img->format->BytesPerPixel) {
    case 4: // has alpha channel
        if (img->format->Rmask == 0x000000ff) {
            // RGBA
        }
        else {
            // BGRA
        }
        break;
    case 3: // no alpha channel
        if (img->format->Rmask == 0x000000ff) {
            // RGB
        }
        else {
            // BGR
        }
        break;
    default:
        cerr << "Warning: image is not truecolor: " << m_identifier << endl;
    }

    // copy data from SDL_Surface to Texture data (always RGB or RGBA)

    // free memory
    SDL_FreeSurface(img);
}
