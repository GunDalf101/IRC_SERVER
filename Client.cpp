#include "Client.hpp"

IRCClient::IRCClient(int client_fd) {
    this->client_fd = client_fd;
    this->nickname = "";
    this->username = "";
    this->password = "";
}

IRCClient::~IRCClient() {
    close(client_fd);
}

void IRCClient::sendMessages(std::string response) {
    std::string message = response + "\r\n";
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