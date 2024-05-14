#pragma once

#include "Client.hpp"
#include "Command.hpp"

class IRCClient;

class CommandFactory {
    public:
        static ICommand *createCommand(const std::string &command)
        {
            if (command == "NICK") {
                return new CommandNick();
            } else if (command == "USER") {
                return new CommandUser();
            } else if (command == "PASS") {
                return new CommandPass();
            } else if (command == "JOIN") {
                return new CommandJoin();
            } else {
                return nullptr;
            }
        }
};