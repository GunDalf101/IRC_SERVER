#pragma once
#include "webserv.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "CommandFactory.hpp"

class IRCServer {
    private:
        int server_fd;
        std::vector<struct pollfd> fds;
        std::unordered_map<int, IRCClient*> clients;

        int setupMainSocket(int port);
        void handleConnection();
        void handleClients(int i);
        void parseCommands(std::string command, int clientFd);
        void sendResponse(int client_fd, std::string response);
        void replaceClient(int client_fd, IRCClient* newClient);
    public:
        IRCServer(int port, std::string password);
        ~IRCServer();
        void run();
};