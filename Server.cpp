#include "Server.hpp"
#include "CommandFactory.hpp"
#include <sys/_pthread/_pthread_once_t.h>

IRCServer::IRCServer(int port, std::string password) {
    this->password = password;
    server_fd = setupMainSocket(port);
    pollfd pfd;
    pfd.fd = server_fd;
    pfd.events = POLLIN;
    fds.push_back(pfd);
    char hostbuffer[256];
    if (gethostname(hostbuffer, sizeof(hostbuffer)) == 0)
        hostname = hostbuffer;
    else
        hostname = "localhost";
}

IRCServer::~IRCServer() {
    close(server_fd);
}

std::string &IRCServer::getPassword() {
    return this->password;
}

void IRCServer::run() {
    while (true) {
        int ret = poll(fds.data(), fds.size(), -1);
        if (ret == -1) {
            std::cerr << "poll() failed: " << strerror(errno) << std::endl;
            return;
        }

        if (fds[0].revents & POLLIN) {
            handleConnection();
        }

        for (size_t i = 1; i < fds.size(); i++) {
            if (fds[i].revents & POLLIN) {
                handleClients(i);
            }
        }
    }
}

int IRCServer::setupMainSocket(int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "socket() failed: " << strerror(errno) << std::endl;
        exit(1);
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        std::cerr << "setsockopt() failed: " << strerror(errno) << std::endl;
        exit(1);
    }
    fcntl(server_fd, F_SETFL, O_NONBLOCK);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        std::cerr << "bind() failed: " << strerror(errno) << std::endl;
        exit(1);
    }

    if (listen(server_fd, 10) == -1) {
        std::cerr << "listen() failed: " << strerror(errno) << std::endl;
        exit(1);
    }

    return server_fd;
}

void IRCServer::handleConnection() {
    struct sockaddr_in client_addr;
    socklen_t client_addrlen = sizeof(client_addr);
    int new_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addrlen);
    char ipCli[INET_ADDRSTRLEN];
    if (new_fd == -1) {
        std::cerr << "accept() failed: " << strerror(errno) << std::endl;
        return;
    }
    inet_ntop(AF_INET, &client_addr.sin_addr, ipCli, INET_ADDRSTRLEN);
    pollfd pfd;
    pfd.fd = new_fd;
    pfd.events = POLLIN;
    fcntl(new_fd, F_SETFL, O_NONBLOCK);
    fds.push_back(pfd);
    IRCClient* client = new IRCClient(new_fd);
    clients[new_fd] = client;
    client->setIpAddr(ipCli);
    std::cout << "New connection from " << ipCli << std::endl;
}

void IRCServer::handleClients(int i) {
    char buffer[1024];
    memset(buffer, 0, 1024);
    int valread = read(fds[i].fd, buffer, 1024);
    if (valread == 0) {
        close(fds[i].fd);
        fds.erase(fds.begin() + i);
    } else {
        std::vector<std::string> commands = split(std::string(buffer), '\n');
        for(size_t j = 0; j < commands.size(); j++)
        {
            std::cout << "Received: " << commands[j] << std::endl;
            parseCommands(commands[j], fds[i].fd);
        }
    }
}

void IRCServer::parseCommands(std::string command, int clientFd) {
    std::stringstream ss(command);
    std::string cmd;
    ss >> cmd;
    std::string params;
    std::getline(ss, params);

    ICommand* commandObj = CommandFactory::createCommand(cmd, this);
    if (commandObj != NULL) {
        if(!params.empty())
            params = params.substr(1, params.length()); // this for removing the space after the command
        if (params[params.length() - 1] == '\r') {
            params = params.substr(0, params.length() - 1);
        }
        commandObj->execute(clients[clientFd], params);
    } else {
        std::cout << "Unknown command: " << cmd << std::endl;
    }
}

void IRCServer::sendResponse(int client_fd, std::string response) {
    std::string message = response + "\r\n";
    send(client_fd, message.c_str(), message.length(), 0);
}

void IRCServer::replaceClient(int client_fd, IRCClient* newClient) {
    if (clients.find(client_fd) != clients.end()) {
        delete clients[client_fd];
        clients[client_fd] = newClient;
    }
}

void IRCServer::createChannel(std::string channelName) {
    if (channels.find(channelName) == channels.end()) {
        channels[channelName] = new IRCChannel(channelName);
    }
}

IRCChannel* IRCServer::getChannel(std::string channelName) {
    if (channels.find(channelName) != channels.end())
        return channels[channelName];
    return NULL;
}

std::map<int, IRCClient*> IRCServer::getCliens()
{
    return this->clients;
}

IRCClient* IRCServer::getClientByNickname(std::string nickname) {
    std::map<int, IRCClient*>::iterator it = clients.begin();
    while (it != clients.end()) {
        std::cout << it->second->getNickname() << std::endl;
        if (it->second->getNickname() == nickname) {
            return it->second;
        }
        it++;
    }
    return NULL;
}

void IRCServer::setHostName(std::string _hostname)
{
    hostname = _hostname;
}

std::string IRCServer::getHostName(void) const
{
    return (hostname);
}
