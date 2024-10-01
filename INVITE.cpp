#include "Command.hpp"

void CommandInvite::execute(IRCClient *client, const std::string &params)
{
    std::string nickname;
    std::string channelName;
    std::stringstream paramsStream(params);
    paramsStream >> nickname >> channelName;

    if(!client->isAuthentificated())
        return client->sendMessages(ERR_NOTREGISTERED(client->getHostname(), client->getNickname()));

    IRCChannel *channel = server->getChannel(channelName);
    if (channel == NULL)
    {
        client->sendMessages(ERR_NOSUCHCHANNEL(client->getHostname(), client->getNickname(), channelName));
        return;
    }
    IRCClient *invitedClient = server->getClientByNickname(nickname);
    if (invitedClient == NULL)
    {
        client->sendMessages(ERR_NOSUCHNICK(client->getHostname(), client->getNickname(), nickname));
        return;
    }
    if (!channel->getOperator(client->getNickname()))
    {
        client->sendMessages(ERR_CHANOPRIVSNEEDED(client->getHostname(), client->getNickname(), channelName));
        return;
    }
    if (channel->getClient(invitedClient->getNickname()))
    {
        client->sendMessages(ERR_USERONCHANNEL(client->getHostname(), client->getNickname(), invitedClient->getNickname(), channelName));
        return;
    }
    invitedClient->invite(channelName);
    client->sendMessages(RPL_INVITING(client->getHostname(), client->getNickname(), invitedClient->getNickname(), channelName));
    invitedClient->sendMessages(RPL_INVITE(client->getNickname(), invitedClient->getUsername(), invitedClient->getHostname(), channelName));
    
}