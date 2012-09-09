/*
 *    <one line to give the library's name and an idea of what it does.>
 *    Copyright (C) 2012  David Cavazos <email>
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 2.1 of the License, or (at your option) any later version.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with this library; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */


#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <iostream>
#include <cassert>

template <class T>
class Singleton {
public:
    static T* instancePtr();
    static T& instance();
    static void destroy();

private:
    static T* ms_singleton;

    // Disable all constructors, assign operator and destructor
    Singleton() {}
    Singleton(const Singleton<T>&) {}
    Singleton& operator=(const Singleton<T>&) {}
    ~Singleton() {}
};


template <class T>
T* Singleton<T>::ms_singleton = 0;


template <class T>
T* Singleton<T>::instancePtr() {
    if (ms_singleton == 0) {
        std::cout << "Creating singleton" << std::endl;
        ms_singleton = new T;
        assert(ms_singleton != 0);
    }
    return ms_singleton;
}

template <class T>
T& Singleton<T>::instance() {
    return *instancePtr();
}

template <class T>
void Singleton<T>::destroy() {
    std::cout << "Destroying singleton" << std::endl;
    delete ms_singleton;
    ms_singleton = 0;
}

#endif // SINGLETON_HPP
