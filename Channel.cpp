#include "Channel.hpp"
#include "Client.hpp"

IRCChannel::IRCChannel(std::string name) : name(name)
{
    topic = "";
    type = "";
    key = "";
    userLimit = 0;
}

IRCChannel::~IRCChannel()
{
}

void IRCChannel::addUser(IRCClient *client)
{
    members.push_back(client);
    users.push_back(client);
    notifyClients(client->getNickname() + " has joined the channel.");
}

void IRCChannel::removeUser(IRCClient *client)
{
    members.erase(std::remove(members.begin(), members.end(), client), members.end());
    users.erase(std::remove(users.begin(), users.end(), client), users.end());
    notifyClients(client->getNickname() + " has left the channel.");
}

void IRCChannel::notifyClients(std::string message)
{
    for (std::vector<IRCClient *>::iterator it = users.begin(); it != users.end(); it++)
    {
        (*it)->sendMessages(message);
    }
    for (std::vector<IRCClient *>::iterator it = operators.begin(); it != operators.end(); it++)
    {
        (*it)->sendMessages(message);
    }
}

void IRCChannel::addOperator(IRCClient *client)
{
    members.push_back(client);
    operators.push_back(client);
}

void IRCChannel::removeOperator(IRCClient *client)
{
    operators.erase(std::remove(operators.begin(), operators.end(), client), operators.end());
    members.erase(std::remove(members.begin(), members.end(), client), members.end());
}

IRCClient *IRCChannel::getClient(std::string nickname) {
    //print all members
    for (std::vector<IRCClient *>::iterator it = members.begin(); it != members.end(); it++) {
        std::cout << (*it)->getNickname() << std::endl;
    }
    for (std::vector<IRCClient *>::iterator it = members.begin(); it != members.end(); it++) {
        if ((*it)->getNickname() == nickname)
            return *it;
    }
    return NULL;
}

IRCClient *IRCChannel::getOperator(std::string nickname) {
    for (std::vector<IRCClient *>::iterator it = operators.begin(); it != operators.end(); it++) {
        if ((*it)->getNickname() == nickname)
            return *it;
    }
    return NULL;
}