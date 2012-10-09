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


#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include "scalar.hpp"

class Vector3;

class Quaternion {
public:
    Quaternion();
    Quaternion(const Quaternion& rhs);
    Quaternion(const scalar_t& w, const scalar_t& x, const scalar_t& y, const scalar_t& z);
    Quaternion(const Vector3& axis, const scalar_t& angle);
    Quaternion(const scalar_t& yaw, const scalar_t& pitch, const scalar_t& roll);

    Quaternion& operator=(const Quaternion& rhs);
    Quaternion operator-() const;
    Quaternion& operator+=(const Quaternion& rhs);
    Quaternion& operator-=(const Quaternion& rhs);
    Quaternion& operator*=(const Quaternion& rhs);
    Quaternion& operator*=(const scalar_t& rhs);
    Quaternion& operator/=(const scalar_t& rhs);
    const Quaternion operator+(const Quaternion& rhs) const;
    const Quaternion operator-(const Quaternion& rhs) const;
    const Quaternion operator*(const Quaternion& rhs) const;
    const Quaternion operator*(const scalar_t& rhs) const;
    const Quaternion operator/(const scalar_t& rhs) const;
    bool operator==(const Quaternion& rhs) const;
    bool operator!=(const Quaternion& rhs) const;

    const scalar_t& getW() const;
    const scalar_t& getX() const;
    const scalar_t& getY() const;
    const scalar_t& getZ() const;
    Vector3 getAxis() const;
    scalar_t getAngle() const;
    scalar_t getYaw() const;
    scalar_t getPitch() const;
    scalar_t getRoll() const;
    void setW(const scalar_t& w);
    void setX(const scalar_t& x);
    void setY(const scalar_t& y);
    void setZ(const scalar_t& z);
    void setValue(const scalar_t& w, const scalar_t& x, const scalar_t& y, const scalar_t& z);
    void setValue(const Quaternion& q);
    void setAxisAngle(const Vector3& axis, const scalar_t& angle);
    void setEuler(const scalar_t& yaw, const scalar_t& pitch, const scalar_t& roll);

    scalar_t length() const;
    scalar_t lengthSquared() const;
    scalar_t angle(const Quaternion& q) const;
    scalar_t dot(const Quaternion& q) const;
    Quaternion inverse() const;
    Quaternion& normalize();
    Quaternion normalized() const;
    Quaternion slerp(const Quaternion& q, const scalar_t& t) const;

private:
    scalar_t m_data[4];
};

const Quaternion QUATERNION_IDENTITY = Quaternion(1, 0, 0, 0);

#include "vector3.hpp"



inline Quaternion::Quaternion() {
}

inline Quaternion::Quaternion(const Quaternion& rhs) {
    setValue(rhs.getW(), rhs.getX(), rhs.getY(), rhs.getZ());
}

inline Quaternion::Quaternion(const scalar_t& w, const scalar_t& x, const scalar_t& y, const scalar_t& z) {
    setValue(w, x, y, z);
}

inline Quaternion::Quaternion(const Vector3& axis, const scalar_t& angle) {
    setAxisAngle(axis, angle);
}

inline Quaternion::Quaternion(const scalar_t& yaw, const scalar_t& pitch, const scalar_t& roll) {
    setEuler(yaw, pitch, roll);
}



inline Quaternion& Quaternion::operator=(const Quaternion& rhs) {
    setValue(rhs.getW(), rhs.getX(), rhs.getY(), rhs.getZ());
    return *this;
}

inline Quaternion Quaternion::operator-() const {
    return Quaternion(-getW(), -getX(), -getY(), -getZ());
}

inline Quaternion& Quaternion::operator+=(const Quaternion& rhs) {
    m_data[0] += rhs.m_data[0];
    m_data[1] += rhs.m_data[1];
    m_data[2] += rhs.m_data[2];
    m_data[3] += rhs.m_data[3];
    return *this;
}

inline Quaternion& Quaternion::operator-=(const Quaternion& rhs) {
    m_data[0] -= rhs.m_data[0];
    m_data[1] -= rhs.m_data[1];
    m_data[2] -= rhs.m_data[2];
    m_data[3] -= rhs.m_data[3];
    return *this;
}

inline Quaternion& Quaternion::operator*=(const Quaternion& rhs) {
    setValue(
        getW() * rhs.getW() - getX() * rhs.getX() - getY() * rhs.getY() - getZ() * rhs.getZ(),
        getW() * rhs.getX() + getX() * rhs.getW() + getY() * rhs.getZ() - getZ() * rhs.getY(),
        getW() * rhs.getY() + getY() * rhs.getW() + getZ() * rhs.getX() - getX() * rhs.getZ(),
        getW() * rhs.getZ() + getZ() * rhs.getW() + getX() * rhs.getY() - getY() * rhs.getX()
    );
    return *this;
}

inline Quaternion& Quaternion::operator*=(const scalar_t& rhs) {
    m_data[0] *= rhs;
    m_data[1] *= rhs;
    m_data[2] *= rhs;
    m_data[3] *= rhs;
    return *this;
}

inline Quaternion& Quaternion::operator/=(const scalar_t& rhs) {
    return *this *= ONE / rhs;
}

inline const Quaternion Quaternion::operator+(const Quaternion& rhs) const {
    return Quaternion(*this) += rhs;
}

inline const Quaternion Quaternion::operator-(const Quaternion& rhs) const {
    return Quaternion(*this) -= rhs;
}

inline const Quaternion Quaternion::operator*(const Quaternion& rhs) const {
    return Quaternion(*this) *= rhs;
}

inline const Quaternion Quaternion::operator*(const scalar_t& rhs) const {
    return Quaternion(*this) *= rhs;
}

inline const Quaternion Quaternion::operator/(const scalar_t& rhs) const {
    return Quaternion(*this) /= rhs;
}

inline bool Quaternion::operator==(const Quaternion& rhs) const {
    return getW() == rhs.getW() &&
           getX() == rhs.getX() &&
           getY() == rhs.getY() &&
           getZ() == rhs.getZ();
}

inline bool Quaternion::operator!=(const Quaternion& rhs) const {
    return !(*this == rhs);
}



inline const scalar_t& Quaternion::getW() const {
    return m_data[0];
}

inline const scalar_t& Quaternion::getX() const {
    return m_data[1];
}

inline const scalar_t& Quaternion::getY() const {
    return m_data[2];
}

inline const scalar_t& Quaternion::getZ() const {
    return m_data[3];
}

inline Vector3 Quaternion::getAxis() const {
    scalar_t s_squared = ONE - getW() * getW();

    if (s_squared < static_cast<scalar_t>(10.0) * EPSILON) //Check for divide by zero
        return VECTOR3_UNIT_X;  // Arbitrary
    scalar_t s = ONE / std::sqrt(s_squared);
    return Vector3(getX() * s, getY() * s, getZ() * s);
}

inline scalar_t Quaternion::getAngle() const {
    return TWO * std::acos(getW());
}

inline void Quaternion::setW(const scalar_t& w) {
    m_data[0] = w;
}

inline void Quaternion::setX(const scalar_t& x) {
    m_data[1] = x;
}

inline void Quaternion::setY(const scalar_t& y) {
    m_data[2] = y;
}

inline void Quaternion::setZ(const scalar_t& z) {
    m_data[3] = z;
}

inline void Quaternion::setValue(const scalar_t& w, const scalar_t& x, const scalar_t& y, const scalar_t& z) {
    setW(w);
    setX(x);
    setY(y);
    setZ(z);
}

inline void Quaternion::setValue(const Quaternion& q) {
    setValue(q.getW(), q.getX(), q.getY(), q.getZ());
}

inline void Quaternion::setAxisAngle(const Vector3& axis, const scalar_t& angle) {
    scalar_t halfAngle = angle * HALF;
    scalar_t s = std::sin(halfAngle) / axis.length();
    setValue(std::cos(halfAngle), axis.getX() * s, axis.getY() * s, axis.getZ() * s);
}

inline void Quaternion::setEuler(const scalar_t& yaw, const scalar_t& pitch, const scalar_t& roll) {
    scalar_t halfYaw = yaw * HALF;
    scalar_t halfPitch = pitch * HALF;
    scalar_t halfRoll = roll * HALF;
    scalar_t cosYaw = std::cos(halfYaw);
    scalar_t sinYaw = std::sin(halfYaw);
    scalar_t cosPitch = std::cos(halfPitch);
    scalar_t sinPitch = std::sin(halfPitch);
    scalar_t cosRoll = std::cos(halfRoll);
    scalar_t sinRoll = std::sin(halfRoll);
    setValue(
        cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw,
        cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw,
        cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw,
        sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw
    );
}



inline scalar_t Quaternion::length() const {
    return std::sqrt(lengthSquared());
}

inline scalar_t Quaternion::lengthSquared() const {
    return dot(*this);
}

inline scalar_t Quaternion::angle(const Quaternion& q) const {
    scalar_t s = std::sqrt(lengthSquared() * q.lengthSquared());
    return std::acos(dot(q) / s);
}

inline scalar_t Quaternion::dot(const Quaternion& q) const {
    return getW() * q.getW() +
           getX() * q.getX() +
           getY() * q.getY() +
           getZ() * q.getZ();
}

inline Quaternion Quaternion::inverse() const {
    return Quaternion(getW(), -getX(), -getY(), -getZ());
}

inline Quaternion& Quaternion::normalize() {
    return *this /= length();
}

inline Quaternion Quaternion::normalized() const {
    return *this / length();
}

inline scalar_t Quaternion::getYaw() const {
    return std::asin(-TWO * (getX() * getZ() - getW() * getY()));
}

inline scalar_t Quaternion::getPitch() const {
    return std::atan2(TWO * (getY() * getZ() + getW() * getX()),
            getW() * getW() - getX() * getX() - getY() * getY() + getZ() * getZ());
}

inline scalar_t Quaternion::getRoll() const {
    return std::atan2(TWO * (getX() * getY() + getW() * getZ()),
            getW() * getW() + getX() * getX() - getY() * getY() - getZ() * getZ());
}

inline Quaternion Quaternion::slerp(const Quaternion& q, const scalar_t& t) const {
    scalar_t product = dot(q) / std::sqrt(lengthSquared() * q.lengthSquared());
    if (std::abs(product) != ONE) {
        // Take care of long angle case see http://en.wikipedia.org/wiki/Slerp
        const scalar_t sign = (product < 0) ? -ONE : ONE;

        const scalar_t theta = std::acos(sign * product);
        const scalar_t s1 = std::sin(sign * t * theta);
        const scalar_t d = ONE / std::sin(theta);
        const scalar_t s0 = std::sin((ONE - t) * theta);

        return Quaternion(
            (getW() * s0 + q.getW() * s1) * d,
            (getX() * s0 + q.getX() * s1) * d,
            (getY() * s0 + q.getY() * s1) * d,
            (getZ() * s0 + q.getZ() * s1) * d
        );
    }
    else
        return *this;
}

#endif // QUATERNION_HPP
