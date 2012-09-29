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


#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include <ostream>
#include <string>
#include <vector>
#include <deque>
#include "tokentable.hpp"
#include "command.hpp"
#include "commandobject.hpp"

class Terminal {
public:
    friend class Command;
    friend class CommandObject;

    static bool getObject(const size_t id, CommandObject*& object);
    static const std::string getObjectName(const size_t idObject);
    static std::string findCommandName(const size_t idCommand);

    static void pushCommand(const Command& cmd);
    static void pushCommand(const std::string& cmdStr);
    static void processCommandsQueue();
    static std::vector<std::string> generateObjectsList(const bool shouldIncludeId = false);
    static std::vector<std::string> generateCommandsList(const bool shouldIncludeId = false);
    static std::vector<std::string> generateAttributesList(const bool shouldIncludeId = false);
    static std::vector<std::string> generateAutocompleteList(const std::string& expression);
    static std::string listsToString();

private:
    typedef std::map<size_t, CommandObject*> obj_ptr_table_t;

    static TokenTable ms_objectsTable;
    static TokenTable ms_commandsTable;
    static TokenTable ms_attributesTable;
    static obj_ptr_table_t ms_objectPointersTable;
    static std::deque<Command> ms_commandsQueue;

    static size_t registerObject(const std::string& objectName, CommandObject* obj);
    static void unregisterObject(const std::string& objectName);
    static std::vector<std::string> generateAutocompleteObjectList(const std::string& object);
    static std::vector<std::string> generateAutocompleteCommandList(const size_t idObject, const std::string& command);
    static std::vector<std::string> generateAutocompleteAttributeList(const size_t idObject, const std::string& attr);
};



inline bool Terminal::getObject(const size_t id, CommandObject*& object) {
    if (id < ms_objectPointersTable.size()) {
        object = ms_objectPointersTable[id];
        if (object != 0)
            return true;
    }
    return false;
}

inline const std::string Terminal::getObjectName(const size_t idObject) {
//     return ms_objectsTable.findName(idObject);
    return ms_objectPointersTable[idObject]->getObjectName();
}

inline std::string Terminal::findCommandName(const size_t idCommand) {
    return ms_commandsTable.findName(idCommand);
}

#endif // TERMINAL_HPP
