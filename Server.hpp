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
        std::map<int, IRCClient*> clients;
        std::map<std::string, IRCChannel*> channels;
        std::map<int, std::string> buffers;
        std::string hostname;

        int setupMainSocket(int port);
        void handleConnection();
        void handleClients(int i);
        void parseCommands(std::string command, int clientFd);
        void sendResponse(int client_fd, std::string response);
    public:
        IRCServer(int port, std::string password);
        std::map<std::string, IRCChannel*> getChannels();
        void replaceClient(int client_fd, IRCClient* newClient);
        void createChannel(std::string channelName);
        void removeChannel(std::string channelName);
        IRCChannel* getChannel(std::string channelName);
        IRCClient* getClientByNickname(std::string nickname);
        std::map<int, IRCClient*> getCliens();
        std::string &getPassword();
        ~IRCServer();
        void run();
        void setHostName(std::string _hostname);
        void welcome_client(IRCClient *client);
        std::string getHostName(void) const;
};