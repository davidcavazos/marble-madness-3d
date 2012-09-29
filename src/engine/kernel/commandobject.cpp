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


#include "engine/kernel/commandobject.hpp"

#include <iostream>
#include <iomanip>
#include "engine/kernel/terminal.hpp"

using namespace std;

const size_t MAX_EXPECTED_ID_DIGITS = 4;

const string SET_COMMAND = "set";

CommandObject::CommandObject(const string& objectName) :
    m_objectName(objectName),
    m_idObject(0),
    m_commands(),
    m_attributes()
{
//     cout << "Creating " << m_objectName << endl;
	m_idObject = Terminal::registerObject(m_objectName, this);
}

CommandObject::~CommandObject() {
    Terminal::unregisterObject(m_objectName);
//     cout << "Destroying " << m_objectName << endl;
}

bool CommandObject::operator<(const CommandObject& rhs) const {
    return m_idObject < rhs.m_idObject;
}

bool CommandObject::operator>(const CommandObject& rhs) const {
    return m_idObject > rhs.m_idObject;
}

bool CommandObject::runObjectCommand(const size_t idCommand, const std::string& arguments) {
    cmd_table_t::iterator it = m_commands.find(idCommand);
    if (it != m_commands.end()) {
        (it->second)(arguments);
        return true;
    }
    cerr << "Object \"" << m_objectName << "\" has no CommandID " << idCommand << endl;
    return false;
}

size_t CommandObject::registerCommand(const string& cmd, const slot_t& slot) {
    size_t id = Terminal::ms_commandsTable.registerToken(cmd);
    cmd_table_t::iterator it = m_commands.find(id);
    if (it == m_commands.end())
        m_commands.insert(pair<size_t, slot_t>(id, slot));
    return id;
}

size_t CommandObject::registerAttribute(const string& attrName, const slot_t& slot) {
    size_t id = Terminal::ms_attributesTable.registerToken(attrName);
    cmd_table_t::iterator it = m_attributes.find(id);
    if (it == m_attributes.end())
        m_attributes.insert(pair<size_t, slot_t>(id, slot));
    registerCommand(SET_COMMAND, boost::bind(&CommandObject::setAttribute, this, _1));
    return id;
}

void CommandObject::setAttribute(const string& arg) {
    size_t id;
    string attrName;
    stringstream ss(arg);
    ss >> attrName;
    if (Terminal::ms_attributesTable.findId(id, attrName)) {
        cmd_table_t::iterator it = m_attributes.find(id);
        if (it != m_attributes.end())
            (it->second)(arg.substr(static_cast<size_t>(ss.tellg())));
    }
}

ostream& operator<<(ostream& out, const CommandObject& rhs) {
    out << setw(MAX_EXPECTED_ID_DIGITS) << rhs.m_idObject << " " << rhs.m_objectName << "   ";
    std::map<size_t, boost::function<void (const std::string&)> >::const_iterator it;
    for (it = rhs.m_commands.begin(); it != rhs.m_commands.end(); ++it)
        out << it->first << " ";
    return out;
}
