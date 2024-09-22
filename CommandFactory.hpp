#pragma once

#include "Client.hpp"
#include "Command.hpp"
#include "Server.hpp"

class IRCClient;

class CommandFactory {
    private:
        static std::string toStandardCommand(const std::string &command)
        {
            std::string standardCommand;
            for (size_t i = 0; i < command.length(); i++)
                standardCommand.append(1, toupper(command.at(i)));
            return standardCommand;
        }
    public:
        static ICommand *createCommand(const std::string &command, IRCServer *server)
        {
            std::string standardCommand = toStandardCommand(command);
            if (standardCommand == "NICK") {
                return new CommandNick(*server);
            } else if (standardCommand == "USER") {
                return new CommandUser();
            } else if (standardCommand == "PASS") {
                return new CommandPass(*server);
            } else if (standardCommand == "JOIN") {
                return new CommandJoin(server);
            } else if (standardCommand == "PART") {
                return new CommandPart(server);
            } else if (standardCommand == "INVITE") {
                return new CommandInvite(server);
            } else if (standardCommand == "KICK") {
                return new CommandKick(server);
            } else if (standardCommand == "TOPIC") {
                return new CommandTopic(server);
            } else if (standardCommand == "PRIVMSG") {
                return new CommandPrivMsg(*server);
            } else if (standardCommand == "MODE") {
                return new CommandMode(*server);
            } else {
                return NULL;
            }
        }
};
