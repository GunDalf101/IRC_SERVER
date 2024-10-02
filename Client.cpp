#include "Client.hpp"

IRCClient::IRCClient(int client_fd) {
    this->client_fd = client_fd;
    this->nickname = "";
    this->username = "";
    this->password = "";
    this->realname = "";
    this->ipAddr = "";
    this->authLevel = 0;
    char hostbuffer[256];
    if (gethostname(hostbuffer, sizeof(hostbuffer)) == 0)
        hostname = hostbuffer;
    else
        hostname = "localhost";
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

bool IRCClient::isInvited(std::string channelName) {
    for (std::vector<std::string>::iterator it = channelInvited.begin(); it != channelInvited.end(); it++) {
        if (*it == channelName)
            return true;
    }
    return false;
}

void IRCClient::invite(std::string channelName) {
    channelInvited.push_back(channelName);
}

void IRCClient::removeInvite(std::string channelName) {
    channelInvited.erase(std::remove(channelInvited.begin(), channelInvited.end(), channelName), channelInvited.end());
}

bool IRCClient::operator==(IRCClient rhs) const
{
    return (getNickname() == rhs.getNickname());
}
