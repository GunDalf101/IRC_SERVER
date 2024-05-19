#include "Channel.hpp"
#include "Client.hpp"

IRCChannel::IRCChannel(std::string name) : name(name)
{
}

IRCChannel::~IRCChannel()
{
}

void IRCChannel::addClient(IRCClient *client)
{
    members.push_back(client);
    notifyClients(client->getNickname() + " has joined the channel.");
}

void IRCChannel::removeClient(IRCClient *client)
{
    members.erase(std::remove(members.begin(), members.end(), client), members.end());
    notifyClients(client->getNickname() + " has left the channel.");
}

void IRCChannel::notifyClients(std::string message)
{
    for (std::vector<IRCClient *>::iterator it = members.begin(); it != members.end(); it++)
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
    operators.push_back(client);
}

void IRCChannel::removeOperator(IRCClient *client)
{
    operators.erase(std::remove(operators.begin(), operators.end(), client), operators.end());
}