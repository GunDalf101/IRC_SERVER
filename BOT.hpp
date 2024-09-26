#pragma once

#include <iostream>
#include <fstream>
#include "Command.hpp"

class BOT : public ICommand 
{
    private:
        IRCServer &server;
        std::string nickname;
        std::string username;
        std::string toLower(std::string &s);
        std::map<std::string, std::string> database;
    public:
        BOT(IRCServer &server):server(server)
        {
            nickname = "BOT";
            username = "BOT";
        };
        void loadDatabase();
        void execute(IRCClient *client, const std::string &params);
        bool canExecute(IRCClient *client){
            (void)client;
            return true;
        }
};