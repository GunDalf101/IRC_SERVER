#pragma once
#include "webserv.hpp"
#include "Client.hpp"
#include "Channel.hpp"

std::vector<std::string> split(const std::string& s, char delimiter);
std::vector<std::string> toReqArgs(const std::string &params);

class ICommand;

class IRCServer {
    private:
        int server_fd;
        std::string password;
        std::vector<struct pollfd> fds;
        std::unordered_map<int, IRCClient*> clients;
        std::unordered_map<std::string, IRCChannel*> channels;
        std::string hostname;

        int setupMainSocket(int port);
        void handleConnection();
        void handleClients(int i);
        void parseCommands(std::string command, int clientFd);
        void sendResponse(int client_fd, std::string response);
    public:
        IRCServer(int port, std::string password);
        void replaceClient(int client_fd, IRCClient* newClient);
        void createChannel(std::string channelName);
        IRCChannel* getChannel(std::string channelName);
        IRCClient* getClientByNickname(std::string nickname);
        std::unordered_map<int, IRCClient*> getCliens();
        std::string &getPassword();
        ~IRCServer();
        void run();
        void setHostName(std::string _hostname);
        std::string getHostName(void) const;
};