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


#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <ostream>
#include <istream>
#include <sstream>
#include <string>

class Terminal;

class Command {
public:
    friend class Terminal;
    friend std::ostream& operator<<(std::ostream& out, const Command& rhs);
    friend std::istream& operator>>(std::istream& in, Command& rhs);

    Command(const std::string& expression);
    Command(const size_t idObject, const size_t idCommand, const std::string& arguments = "");

    size_t getIdObject() const;
    size_t getIdCommand() const;
    const std::string& getArguments() const;
    template <typename T>
    void setArguments(const T& arguments);
    void setArguments(const char* const arguments);
    void setArguments(const std::string& arguments);

    void appendToArguments(const std::string& argsAppended);
    bool parseCommand(const std::string& expression);

private:
    size_t m_idObject;
    size_t m_idCommand;
    std::string m_arguments;

    bool run() const;
};



inline size_t Command::getIdObject() const {
    return m_idObject;
}

inline size_t Command::getIdCommand() const {
    return m_idCommand;
}

inline const std::string& Command::getArguments() const {
    return m_arguments;
}

template <typename T>
void Command::setArguments(const T& arguments) {
    std::ostringstream ss;
    ss << arguments;
    m_arguments = ss.str();
}

inline void Command::setArguments(const char* const arguments) {
    m_arguments = std::string(arguments);
}

inline void Command::setArguments(const std::string& arguments) {
    m_arguments = arguments;
}

#endif // COMMAND_HPP
