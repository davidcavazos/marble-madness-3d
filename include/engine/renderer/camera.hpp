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


#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "engine/kernel/component.hpp"

typedef enum {
    CAMERA_ORTHOGRAPHIC,
    CAMERA_PROJECTION
} camera_t;

typedef struct {
    int posX;
    int posY;
    size_t width;
    size_t height;
} viewport_t;

class Camera: public Component {
public:
    Camera(Entity* const entity, const camera_t type);
    ~Camera();

    camera_t getType() const;
    const viewport_t& getViewport() const;
    float getAspectRatio() const;
    float getPerspectiveFOV() const;
    float getOrthoWidth() const;
    float getOrthoHeight() const;
    float getNearDistance() const;
    float getFarDistance() const;
    void setType(const camera_t type);
    void setViewport(const int posX, const int posY, const size_t width, const size_t height);
    void setPerspectiveFOV(const float fov);
    void setOrthoHeight(const float orthoHeight);
    void setNearDistance(const float nearDistance);
    void setFarDistance(const float farDistance);

private:
    camera_t m_type;
    viewport_t m_viewport;
    float m_aspectRatio;
    float m_perspectiveFOV;
    float m_orthoHeight;
    float m_nearDistance;
    float m_farDistance;
};



inline camera_t Camera::getType() const {
    return m_type;
}

inline const viewport_t& Camera::getViewport() const {
    return m_viewport;
}

inline float Camera::getAspectRatio() const {
    return m_aspectRatio;
}

inline float Camera::getPerspectiveFOV() const {
    return m_perspectiveFOV;
}

inline float Camera::getOrthoWidth() const {
    return m_orthoHeight * m_aspectRatio;
}

inline float Camera::getOrthoHeight() const {
    return m_orthoHeight;
}

inline float Camera::getNearDistance() const {
    return m_nearDistance;
}

inline float Camera::getFarDistance() const {
    return m_farDistance;
}

inline void Camera::setType(const camera_t type) {
    m_type = type;
}

inline void Camera::setViewport(const int posX, const int posY, const size_t width, const size_t height) {
    m_viewport.posX = posX;
    m_viewport.posY = posY;
    m_viewport.width = width;
    m_viewport.height = height;
    m_aspectRatio = static_cast<float>(m_viewport.width) / m_viewport.height;
}

inline void Camera::setPerspectiveFOV(const float fov) {
    m_perspectiveFOV = fov;
}

inline void Camera::setOrthoHeight(const float orthoHeight) {
    m_orthoHeight = orthoHeight;
}

inline void Camera::setNearDistance(const float nearDistance) {
    if (nearDistance < m_farDistance)
        m_nearDistance = nearDistance;
}

inline void Camera::setFarDistance(const float farDistance) {
    if (farDistance > m_nearDistance)
        m_farDistance = farDistance;
}

#endif // CAMERA_HPP
