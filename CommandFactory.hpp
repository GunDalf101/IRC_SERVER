#pragma once

#include "Client.hpp"
#include "Command.hpp"
#include "Server.hpp"

class IRCClient;

class CommandFactory {
    public:
        static ICommand *createCommand(const std::string &command, IRCServer *server)
        {
            if (command == "NICK") {
                return new CommandNick();
            } else if (command == "USER") {
                return new CommandUser();
            } else if (command == "PASS") {
                return new CommandPass();
            } else if (command == "JOIN") {
                return new CommandJoin(server);
            } else if (command == "PART") {
                return new CommandPart(server);
            } else {
                return NULL;
            }
        }
};