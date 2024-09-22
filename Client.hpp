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
        std::vector<std::string> channelInvited;
        std::string line;
        std::string ipAddr;
    public:

        IRCClient(int client_fd);
        virtual ~IRCClient();

        unsigned int authLevel;
        int getClientFd() { return client_fd; }
        std::string getNickname() const { return nickname; }
        std::string getUsername() { return username; }
        std::string getPassword() { return password; }
        void setNickname(std::string nickname) { this->nickname = nickname; }
        void setUsername(std::string username) { this->username = username; }
        void setPassword(std::string password) { this->password = password; }
        bool isAuthentificated() { return this->authLevel == 3; }
        std::string getHostname() { return hostname; }
        void setHostname(std::string hostname) { this->hostname = hostname; }
        std::string getRealname() { return realname; }
        void setRealname(std::string realname) { this->realname = realname; }
        std::string getLine() { return line; }
        void setLine(std::string line) { this->line = line; }
        std::string getIpAddr() { return ipAddr; }
        void setIpAddr(std::string ipAddr) { this->ipAddr = ipAddr; }
        bool isInvited(std::string channelName);
        void invite(std::string channelName);
        void removeInvite(std::string channelName);
        bool operator==(IRCClient rhs) const;


        virtual void sendMessages(std::string message);
        std::string receiveMessages();
};