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


#ifndef COMMANDOBJECT_HPP
#define COMMANDOBJECT_HPP

#include <string>
#include <sstream>
#include <map>
#include <boost/function.hpp>
#include <boost/bind.hpp>

class Command;

class CommandObject {

public:
    friend std::ostream& operator<<(std::ostream& out, const CommandObject& rhs);

    typedef boost::function<void (const std::string&)> slot_t;
    typedef std::map<size_t, slot_t> cmd_table_t;

    CommandObject(const std::string& objectName);
    virtual ~CommandObject();
    bool operator<(const CommandObject& rhs) const;
    bool operator>(const CommandObject& rhs) const;

    size_t getIdObject() const;
    const std::string& getObjectName() const;

    bool isCommandFound(const size_t idCommand) const;
    bool isAttributeFound(const size_t idAttribute) const;

    bool runObjectCommand(const size_t idCommand, const std::string& arguments);

    size_t registerCommand(const std::string& cmd, const slot_t& slot);
    size_t registerAttribute(const std::string& attrName, const slot_t& slot);

protected:
    std::string m_objectName;
    size_t m_idObject;

private:
    cmd_table_t m_commands;
    cmd_table_t m_attributes;

    void setAttribute(const std::string& arg);
};



inline size_t CommandObject::getIdObject() const {
    return m_idObject;
}

inline const std::string& CommandObject::getObjectName() const {
    return m_objectName;
}

inline bool CommandObject::isCommandFound(const size_t idCommand) const {
    cmd_table_t::const_iterator it = m_commands.find(idCommand);
    if (it != m_commands.end())
        return true;
    return false;
}

inline bool CommandObject::isAttributeFound(const size_t idAttribute) const {
    cmd_table_t::const_iterator it = m_attributes.find(idAttribute);
    if (it != m_attributes.end())
        return true;
    return false;
}

template <typename T>
inline void strToVal(const std::string& str, T& val) {
    std::stringstream ss(str);
    ss >> val;
}

#endif // COMMANDOBJECT_HPP
