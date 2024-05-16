#include "Command.hpp"
#include <iostream>

void CommandNick::execute(IRCClient *client, const std::string &params)
{
    std::string oldNick = client->getNickname();
    std::stringstream paramsStream(params);
    std::string nick;
    paramsStream >> nick;
    std::cout << "nick: " << nick << std::endl;
    if (nick == ":")
    {
        client->sendMessages(ERR_NONICKNAMEGIVEN(nick, client->getHostname()));
        return;
    }
    client->setNickname(nick);
    client->sendMessages(NICKNAME_RPLY(oldNick, client->getUsername(), client->getHostname(), nick));
}

void CommandUser::execute(IRCClient *client, const std::string &params)
{
    std::string username;
    std::string mode;
    std::string unused;
    std::string realname;
    std::stringstream paramsStream(params);
    paramsStream >> username >> mode >> unused >> realname;
    client->setUsername(username);
    client->setRealname(realname);
}

void CommandPass::execute(IRCClient *client, const std::string &params)
{
    std::string password;
    std::stringstream paramsStream(params);
    paramsStream >> password;
    client->setPassword(password);
}

void CommandJoin::execute(IRCClient *client, const std::string &params)
{
    std::string channelName = params;
    if (server->getChannel(channelName) == NULL)
    {
        server->createChannel(channelName);
    }
    IRCChannel *channel = server->getChannel(channelName);
    channel->addClient(client);
    client->sendMessages(RPL_JOIN(client->getNickname(), client->getUsername(), client->getHostname(), channelName));
}

void CommandPart::execute(IRCClient *client, const std::string &params)
{
    std::string channelName = params;
    IRCChannel *channel = server->getChannel(channelName);
    if (channel == NULL)
    {
        client->sendMessages(":" + client->getNickname() + " 403 " + client->getNickname() + " " + channelName + " :No such channel");
        return;
    }
    channel->removeClient(client);
    client->sendMessages(":" + client->getNickname() + "!~" + client->getUsername() + "@" + client->getHostname() + " PART " + channelName);
}