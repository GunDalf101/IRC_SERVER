#pragma once
#include "Server.hpp"
#include <unordered_map>

class IRCClient;

class ICommand {
    public:
        virtual ~ICommand() {}
        virtual void execute(IRCClient *client, const std::string &params) = 0;
        virtual bool canExecute(IRCClient *client) = 0;
};

class CommandNick : public ICommand {
    IRCServer &server;
    public:
        CommandNick(IRCServer &server):server(server){};
        void execute(IRCClient *client, const std::string &params);
        bool isAlreadyInuse(const std::string &new_nickname);
        bool isValidNickUser(const std::string &nickname);
        bool canExecute(IRCClient *client){
            (void)client;
            return true;
        }
};

class CommandUser : public ICommand {
    public:
        void execute(IRCClient *client, const std::string &params);
        bool canExecute(IRCClient *client){
            (void)client;
            return true;
        }
};

class CommandPass : public ICommand {
    private:
        IRCServer &server;
    public:
        CommandPass(IRCServer &server):server(server){}
        void execute(IRCClient *client, const std::string &params);
        bool canExecute(IRCClient *client){
            (void)client;
            return true;
        }
};

class CommandJoin : public ICommand {
    private:
        IRCServer *server;
    public:
        CommandJoin(IRCServer *server) : server(server) {}
        void execute(IRCClient *client, const std::string &params);
        void checkChannel(std::unordered_map<std::string, std::string> channelKeyMap, IRCClient *client);
        bool canExecute(IRCClient *client){
            return client->isAuthentificated();
        }
        void setServer(IRCServer *server){
            this->server = server;
        }
};

class CommandPart : public ICommand {
    private:
        IRCServer *server;
    public:
        CommandPart(IRCServer *server) : server(server) {}
        void execute(IRCClient *client, const std::string &params);
        bool canExecute(IRCClient *client){
            return client->isAuthentificated();
        }
        void setServer(IRCServer *server){
            this->server = server;
        }
};