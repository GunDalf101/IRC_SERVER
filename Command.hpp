#pragma once
#include "Client.hpp"

class IRCClient;

class ICommand {
    public:
        virtual ~ICommand() {}
        virtual void execute(IRCClient *client, const std::string &params) = 0;
};

class CommandNick : public ICommand {
    public:
        void execute(IRCClient *client, const std::string &params);
};

class CommandUser : public ICommand {
    public:
        void execute(IRCClient *client, const std::string &params);
};

class CommandPass : public ICommand {
    public:
        void execute(IRCClient *client, const std::string &params);
};

class CommandJoin : public ICommand {
    public:
        void execute(IRCClient *client, const std::string &params);
};
