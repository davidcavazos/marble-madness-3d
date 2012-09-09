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


#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <boost/unordered_set.hpp>

template <class Product, typename IdKey>
class Factory {
public:
    Factory();
    virtual ~Factory() {}

    Product* createProduct(const IdKey key);
    void destroyProduct(Product* product);

protected:
    virtual Product* newProduct(const IdKey key) = 0;

private:
    boost::unordered_set<Product*> m_products;
};


template <class Product, typename IdKey>
Factory<Product, IdKey>::Factory() :
    m_products()
{}

template <class Product, typename IdKey>
Product* Factory<Product, IdKey>::createProduct(const IdKey key) {
    Product* product = newProduct(key);
    m_products.insert(product);
    return product;
}

template <class Product, typename IdKey>
void Factory<Product, IdKey>::destroyProduct(Product* product) {
    m_products.erase(product);
}

#endif // FACTORY_HPP
