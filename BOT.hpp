#pragma once

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>

std::vector<std::string> toReqArgs(const std::string &params);

class BOT 
{
    private:
        int port;
        std::string host;
        std::string password;
        int clientSocket;
        bool isAuth;

        BOT();
        bool connectToServer();
        bool auth();
        size_t sendMessage(std::string message);
        std::string recvLine();
        void parseReply(std::string &reply);
    public:
        BOT(std::string host, int port, std::string password): host(host), port(port), password(password){}
        ~BOT();
        bool getIsAuth(){ return isAuth; }
        bool run();
};