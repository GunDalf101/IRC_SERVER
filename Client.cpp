#include "Client.hpp"

IRCClient::IRCClient(int client_fd) {
    this->client_fd = client_fd;
    this->nickname = "";
    this->username = "user";
    this->password = "";
    this->hostname = "localhost";
    this->realname = "";
    this->authentificated = false;
}

IRCClient::~IRCClient() {
    close(client_fd);
}

void IRCClient::sendMessages(std::string response) {
    std::string message = response + "\r\n";
    std::cout << "send: " << message << std::endl;
    if (send(client_fd, message.c_str(), message.length(), 0) == -1) {
        std::cerr << "send() failed: " << strerror(errno) << std::endl;
    }
}

std::string IRCClient::receiveMessages() {
    char buffer[1024] = {0};
    int valread = read(client_fd, buffer, 1024);
    if (valread == 0) {
        close(client_fd);
        return "";
    }
    return std::string(buffer);
}