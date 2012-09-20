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


#ifndef UNIQUEIDGENERATOR_HPP
#define UNIQUEIDGENERATOR_HPP

#include <set>
#include <iostream>

template <typename T>
class UniqueIdGenerator {
public:
    UniqueIdGenerator(const T& start = 0, const T& step = 1);

    T& getStart() const;
    T& getStep() const;
    T& getLastId() const;
    void setStart(const T& start);
    void setStep(const T& step);

    T nextId();
    void removeId(const T& id);
    void printAvailableIds() const;

private:
    T m_start;
    T m_step;
    T m_lastId;
    std::set<T> m_availableIds;
};



template <typename T>
UniqueIdGenerator<T>::UniqueIdGenerator(const T& start, const T& step) :
    m_start(start),
    m_step(step),
    m_lastId(start),
    m_availableIds()
{}

template <typename T>
inline T& UniqueIdGenerator<T>::getStart() const {
    return m_start;
}

template <typename T>
inline T& UniqueIdGenerator<T>::getStep() const {
    return m_step;
}

template <typename T>
inline T& UniqueIdGenerator<T>::getLastId() const {
    return m_lastId;
}

template <typename T>
inline void UniqueIdGenerator<T>::setStart(const T& start) {
    m_start = start;
}

template <typename T>
inline void UniqueIdGenerator<T>::setStep(const T& step) {
    m_step = step;
}

template <typename T>
T UniqueIdGenerator<T>::nextId() {
    T id;
    if (m_availableIds.empty()) {
        id = m_lastId;
        m_lastId += m_step;
    }
    else {
        id = *m_availableIds.begin();
        m_availableIds.erase(m_availableIds.begin());
    }
    return id;
}

template <typename T>
void UniqueIdGenerator<T>::removeId(const T& id) {
    m_availableIds.insert(id);
    bool isSimplified = false;
    while (!m_availableIds.empty() && !isSimplified) {
        typename std::set<T>::iterator it = m_availableIds.end();
        --it;
        if (m_lastId - m_step <= *it) {
            m_lastId -= m_step;
            m_availableIds.erase(it);
        }
        else
            isSimplified = true;
    }
}

template <typename T>
void UniqueIdGenerator<T>::printAvailableIds() const {
    std::cout << "Available IDs: ";
    typename std::set<T>::iterator it;
    for (it = m_availableIds.begin(); it != m_availableIds.end(); ++it)
        std::cout << *it << ", ";
    std::cout << m_lastId << "+" << std::endl;
}

#endif // UNIQUEIDGENERATOR_HPP
