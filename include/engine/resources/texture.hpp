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


#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <vector>

class SDL_Surface;

typedef enum {
    TEXTURE_FORMAT_RGBA,
    TEXTURE_FORMAT_BGRA,
    TEXTURE_FORMAT_RGB,
    TEXTURE_FORMAT_BGR
} texture_format_t;

class Texture {
public:
    Texture(const std::string& fileName);
    ~Texture();

    const std::string& getFileName() const;
    size_t getId() const;
    size_t getBytesPerPixel() const;
    size_t getWidth() const;
    size_t getHeight() const;
    texture_format_t getTextureFormat() const;
    void* getPixels() const;

    void load();
    void load(const std::string& fileName);

private:
    std::string m_fileName;
    size_t m_textureId;
    SDL_Surface* m_image;
    size_t m_bytesPerPixel;
    size_t m_width;
    size_t m_height;
    texture_format_t m_textureFormat;
    void* m_pixels;

    Texture(const Texture& rhs);
    Texture& operator=(const Texture& rhs);
};



inline const std::string& Texture::getFileName() const {
    return m_fileName;
}

inline size_t Texture::getId() const {
    return m_textureId;
}

inline size_t Texture::getBytesPerPixel() const {
    return m_bytesPerPixel;
}

inline size_t Texture::getWidth() const {
    return m_width;
}

inline size_t Texture::getHeight() const {
    return m_height;
}

inline texture_format_t Texture::getTextureFormat() const {
    return m_textureFormat;
}

inline void* Texture::getPixels() const {
    return m_pixels;
}



inline void Texture::load() {
    load(m_fileName);
}

#endif // TEXTURE_HPP
