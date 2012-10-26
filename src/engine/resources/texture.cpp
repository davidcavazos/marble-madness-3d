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
#include "engine/renderer/rendermanager.hpp"
#include "engine/renderer/renderer.hpp"

using namespace std;

Texture::Texture(const string& fileName):
    m_fileName(fileName),
    m_textureId(0),
    m_bytesPerPixel(0),
    m_width(0),
    m_height(0),
    m_textureFormat(TEXTURE_FORMAT_RGBA),
    m_pixels(0)
{}

Texture::~Texture() {
    RenderManager::getRenderer().deleteTexture(m_textureId);
}

void Texture::load(const string& fileName) {
    SDL_Surface* img = IMG_Load(fileName.c_str());
    if (img == 0) {
        cerr << "Error opening image file: " << fileName << endl;
        return;
    }

    // set attributes
    m_bytesPerPixel = img->format->BytesPerPixel;
    m_width = img->w;
    m_height = img->h;
    m_pixels = img->pixels;

    // check width and height
    if ((m_width & (m_width - 1)) != 0)
        cerr << "Warning: image's width is not a power of 2: " << fileName << endl;
    if ((m_height & (m_height - 1)) != 0)
        cerr << "Warning: image's height is not a power of 2: " << fileName << endl;

    // check for number of channels in each pixel
    switch (m_bytesPerPixel) {
    case 4: // has alpha channel
        if (img->format->Rmask == 0x000000ff)
            m_textureFormat = TEXTURE_FORMAT_RGBA;
        else
            m_textureFormat = TEXTURE_FORMAT_BGRA;
        break;
    case 3: // no alpha channel
        if (img->format->Rmask == 0x000000ff)
            m_textureFormat = TEXTURE_FORMAT_RGB;
        else
            m_textureFormat = TEXTURE_FORMAT_BGR;
        break;
    default:
        cerr << "Warning: image is not truecolor: " << fileName << endl;
    }

    RenderManager::getRenderer().loadTexture(m_textureId, m_bytesPerPixel, m_width,
                                             m_height, m_textureFormat, m_pixels);
    SDL_FreeSurface(img);
}

Texture::Texture(const Texture& rhs):
    m_fileName(rhs.m_fileName),
    m_textureId(rhs.m_textureId),
    m_bytesPerPixel(rhs.m_bytesPerPixel),
    m_width(rhs.m_width),
    m_height(rhs.m_height),
    m_textureFormat(rhs.m_textureFormat),
    m_pixels(rhs.m_pixels)
{}

Texture& Texture::operator=(const Texture& rhs) {
    if (this == &rhs)
        return *this;
    m_fileName = rhs.m_fileName;
    m_textureId = rhs.m_textureId;
    m_bytesPerPixel = rhs.m_bytesPerPixel;
    m_width = rhs.m_width;
    m_height = rhs.m_height;
    m_textureFormat = rhs.m_textureFormat;
    m_pixels = rhs.m_pixels;
    return *this;
}
