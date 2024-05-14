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
        bool authentificated;
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
        std::string getHostname() { return hostname; }
        void setHostname(std::string hostname) { this->hostname = hostname; }
        std::string getRealname() { return realname; }
        void setRealname(std::string realname) { this->realname = realname; }

        virtual void sendMessages(std::string message);
        std::string receiveMessages();
        virtual bool isAuthentificated() { return false; }
        virtual bool isOperator() { return false; }

};