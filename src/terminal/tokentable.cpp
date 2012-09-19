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


#include "terminal/tokentable.hpp"

#include <iostream>
#include <sstream>

using namespace std;

TokenTable::TokenTable() :
    m_tokenMap(),
    m_idGenerator()
{}

size_t TokenTable::registerToken(const string& token) {
    size_t id;
    map<string, size_t>::iterator it = m_tokenMap.find(token);
    if (it == m_tokenMap.end()) {
        id = m_idGenerator.nextId();
        m_tokenMap.insert(pair<string, size_t>(token, id));
    }
    else
        id = it->second;
    return id;
}

void TokenTable::unregisterToken(const string& token) {
    map<string, size_t>::iterator it = m_tokenMap.find(token);
    if (it != m_tokenMap.end()) {
        m_idGenerator.removeId(it->second);
        m_tokenMap.erase(it);
    }
}

bool TokenTable::findId(size_t& id, const string& token) {
    map<string, size_t>::iterator it = m_tokenMap.find(token);
    if (it != m_tokenMap.end()) {
        id = it->second;
        return true;
    }
    cerr << "Token \"" << token << "\" not found!" << endl;
    id = 0;
    return false;
}

vector<string> TokenTable::generateList(const bool shouldIncludeId) const {
    vector<string> tokenList(m_tokenMap.size());
    size_t i = 0;
    map<string, size_t>::const_iterator it;
    for (it = m_tokenMap.begin(); it != m_tokenMap.end(); ++it) {
        stringstream ss;
        ss << it->first;
        if (shouldIncludeId)
            ss << " #" << it->second;
        tokenList[i] = ss.str();
        ++i;
    }
    return tokenList;
}

vector<string> TokenTable::autocompleteList(const std::string& token) const {
    vector<string> autocomplete;
    map<string, size_t>::const_iterator it;
    for (it = m_tokenMap.lower_bound(token); it != m_tokenMap.end(); ++it) {
        if (token.size() > it->first.size())
            break;
        bool isDifferent = false;
        for (size_t i = 0; i < token.size() && !isDifferent; ++i) {
            if (token[i] != it->first[i])
                isDifferent = true;
        }
        if (!isDifferent)
            autocomplete.push_back(it->first);
        else
            break;
    }
    return autocomplete;
}
