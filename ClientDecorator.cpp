#include "ClientDecorator.hpp"

Operator::Operator(IRCClient *client) : AuthenticatedClient(client)
{
}

Operator::~Operator()
{
}

AuthenticatedClient::AuthenticatedClient(IRCClient *client) : IRCClient(client->getClientFd()), wrappedClient(client)
{
    this->setNickname(client->getNickname());
    this->setUsername(client->getUsername());
    this->setPassword(client->getPassword());
    this->setAuthentificated(true);
}

AuthenticatedClient::~AuthenticatedClient()
{
    delete wrappedClient;
}