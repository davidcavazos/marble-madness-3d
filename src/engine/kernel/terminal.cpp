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


#include "engine/kernel/terminal.hpp"

#include <iostream>
#include <sstream>
#include "engine/kernel/commandobject.hpp"

using namespace std;

TokenTable Terminal::ms_objectsTable = TokenTable();
TokenTable Terminal::ms_commandsTable = TokenTable();
TokenTable Terminal::ms_attributesTable = TokenTable();
Terminal::obj_ptr_table_t Terminal:: ms_objectPointersTable = obj_ptr_table_t();
deque<Command> Terminal::ms_commandsQueue = deque<Command>();

enum token_state_t {
    TOKEN_OBJECT,
    TOKEN_COMMAND,
    TOKEN_ARGUMENTS
};

bool charIsSpace(const char c) {
    switch (c) {
    case ' ': case '\t': case '\n': case '\r':
        return true;
    }
    return false;
}

void parse(const string& expression, size_t& i, string& token, token_state_t& curState, const token_state_t nextState) {
    if (charIsSpace(expression[i])) {
        token.append(" ");
        curState = nextState;
        while (i < expression.size() && charIsSpace(expression[i]))
            ++i;
        --i;
    }
    else
        token.push_back(expression[i]);
}

void Terminal::pushCommand(const Command& cmd) {
    ms_commandsQueue.push_back(cmd);
}

void Terminal::pushCommand(const string& cmdStr) {
    Command cmd(0, 0, "");
    if (cmd.parseCommand(cmdStr))
        pushCommand(cmd);
}

void Terminal::processCommandsQueue() {
    while (!ms_commandsQueue.empty()) {
//         cout << ms_commandsQueue.front() << endl;
        ms_commandsQueue.front().run();
        ms_commandsQueue.pop_front();
    }
}

vector<string> Terminal::generateObjectsList(const bool shouldIncludeId) {
    return ms_objectsTable.generateList(shouldIncludeId);
}

vector<string> Terminal::generateCommandsList(const bool shouldIncludeId) {
    return ms_commandsTable.generateList(shouldIncludeId);
}

vector<string> Terminal::generateAttributesList(const bool shouldIncludeId) {
    return ms_attributesTable.generateList(shouldIncludeId);
}

vector<string> Terminal::generateAutocompleteList(const std::string& expression) {
    vector<string> list;

    string object;
    string command;
    string arguments;

    token_state_t tokenState = TOKEN_OBJECT;
    size_t i = 0;
    while (charIsSpace(expression[i]))
        ++i;
    while (i < expression.size()) {
        switch (tokenState) {
        case TOKEN_OBJECT:
            parse(expression, i, object, tokenState, TOKEN_COMMAND);
            break;
        case TOKEN_COMMAND:
            parse(expression, i, command, tokenState, TOKEN_ARGUMENTS);
            break;
        case TOKEN_ARGUMENTS:
            parse(expression, i, arguments, tokenState, TOKEN_ARGUMENTS);
            break;
        default:
            cerr << "Invalid token state: " << tokenState << endl;
        }
        ++i;
    }

    size_t idObject = 0;
    if (command[command.size()-1] == ' ') {
        ms_objectsTable.findId(idObject, object.substr(0, object.size() - 1));
        list = generateAutocompleteAttributeList(idObject, arguments);
    }
    else if (object[object.size()-1] == ' ') {
        ms_objectsTable.findId(idObject, object.substr(0, object.size() - 1));
        list = generateAutocompleteCommandList(idObject, command);
    }
    else
        list = generateAutocompleteObjectList(object);
    return list;
}

size_t Terminal::registerObject(const std::string& objectName, CommandObject* obj) {
    size_t id = ms_objectsTable.registerToken(objectName);
    ms_objectPointersTable.insert(pair<size_t, CommandObject*>(id, obj));
    return id;
}

void Terminal::unregisterObject(const std::string& objectName) {
    size_t id;
    ms_objectsTable.findId(id, objectName);
    obj_ptr_table_t::iterator it = ms_objectPointersTable.find(id);
    if (it != ms_objectPointersTable.end()) {
        ms_objectsTable.unregisterToken(objectName);
        for (size_t i = 0; i < ms_commandsQueue.size(); ++i) {
            if (ms_commandsQueue[i].getIdObject() == id) {
                ms_commandsQueue.erase(ms_commandsQueue.begin() + i);
                --i;
            }
        }
        ms_objectPointersTable.erase(it);
    }
}

vector<string> Terminal::generateAutocompleteObjectList(const string& object) {
    return ms_objectsTable.autocompleteList(object);
}

vector<string> Terminal::generateAutocompleteCommandList(const size_t idObject, const string& command) {
    vector<string> list, completeList;
    size_t idCommand;
    CommandObject* obj = ms_objectPointersTable.find(idObject)->second;
    completeList = ms_commandsTable.autocompleteList(command);
    for (size_t i = 0; i < completeList.size(); ++i) {
        if (ms_commandsTable.findId(idCommand, completeList[i])) {
            if (obj->isCommandFound(idCommand))
                list.push_back(completeList[i]);
        }
    }
    return list;
}

vector< string > Terminal::generateAutocompleteAttributeList(const size_t idObject, const string& attr) {
    vector<string> list, completeList;
    size_t idAttr;
    CommandObject* obj = ms_objectPointersTable.find(idObject)->second;
    completeList = ms_attributesTable.autocompleteList(attr);
    for (size_t i = 0; i < completeList.size(); ++i) {
        if (ms_attributesTable.findId(idAttr, completeList[i])) {
            if (obj->isAttributeFound(idAttr))
                list.push_back(completeList[i]);
        }
    }
    return list;
}

string Terminal::listsToString() {
    stringstream ss;

    ss << "Objects:" << endl;
    obj_ptr_table_t::iterator it;
    for (it = ms_objectPointersTable.begin(); it != ms_objectPointersTable.end(); ++it)
        ss << "    " << *it->second << endl;
    ss << endl;

    ss << "Commands:" << endl;
    vector<string> list = generateCommandsList(true);
    for (size_t i = 0; i < list.size(); ++i)
        ss << "    " << list[i] << endl;
//     ss << endl;
    return ss.str();
}
