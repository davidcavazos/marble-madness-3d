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


#include "engine/kernel/command.hpp"

#include <sstream>
#include "engine/kernel/terminal.hpp"
#include "engine/kernel/commandobject.hpp"

using namespace std;

Command::Command(const size_t idObject, const size_t idCommand, const std::string& arguments) :
    m_idObject(idObject),
    m_idCommand(idCommand),
    m_arguments(arguments)
{}

void Command::appendToArguments(const string& argsAppended) {
    m_arguments.append(" ");
    m_arguments.append(argsAppended);
}

bool Command::parseCommand(const string& expression) {
    string object;
    string command;

    stringstream exp(expression);
    exp >> object >> command >> m_arguments;

    while (exp.good()) {
        string temp;
        exp >> temp;
        if (!temp.empty()) {
            m_arguments.append(" ");
            m_arguments.append(temp);
        }
    }

    if (Terminal::ms_objectsTable.findId(m_idObject, object))
        return Terminal::ms_commandsTable.findId(m_idCommand, command);
    return false;
}

bool Command::run() const {
    CommandObject* object;
    if (Terminal::getObject(m_idObject, object))
        return object->runObjectCommand(m_idCommand, m_arguments);
    cerr << "ObjectID " << m_idObject << " not found!" << endl;
    return false;
}

ostream& operator<<(ostream& out, const Command& rhs) {
//     out << rhs.m_idObject << " " << rhs.m_idCommand << " " << rhs.m_arguments;
    out << Terminal::getObjectName(rhs.m_idObject) << " " <<
            Terminal::findCommandName(rhs.m_idCommand) << " " << rhs.m_arguments;
    return out;
}

istream& operator>>(istream& in, Command& rhs) {
    in >> rhs.m_idObject >> rhs.m_idCommand;
    rhs.m_arguments.clear();
    string temp;
    while (in.good()) {
        in >> temp;
        rhs.m_arguments.append(temp + " ");
    }
    return in;
}
