#pragma once
#include "webserv.hpp"

class IRCClient {
    private:
        int client_fd;
        std::string nickname;
        std::string username;
        std::string password;
        std::string hostname;
        std::string realname;
        std::string line;
        bool authentificated;
        bool operatorFlag;
        std::string ipAddr;
    public:

        IRCClient(int client_fd);
        virtual ~IRCClient();

        int getClientFd() { return client_fd; }
        std::string getNickname() { return nickname; }
        std::string getUsername() { return username; }
        std::string getPassword() { return password; }
        void setNickname(std::string nickname) { this->nickname = nickname; }
        void setUsername(std::string username) { this->username = username; }
        void setPassword(std::string password) { this->password = password; }
        void setAuthentificated(bool authentificated) { this->authentificated = authentificated; }
        bool isAuthentificated() { return authentificated; }
        void setOperator(bool operatorFlag) { this->operatorFlag = operatorFlag; }
        bool isOperator() { return operatorFlag; }
        std::string getHostname() { return hostname; }
        void setHostname(std::string hostname) { this->hostname = hostname; }
        std::string getRealname() { return realname; }
        void setRealname(std::string realname) { this->realname = realname; }
        std::string getLine() { return line; }
        void setLine(std::string line) { this->line = line; }
        std::string getIpAddr() { return ipAddr; }
        void setIpAddr(std::string ipAddr) { this->ipAddr = ipAddr; }


        virtual void sendMessages(std::string message);
        std::string receiveMessages();
};