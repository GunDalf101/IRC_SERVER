#include "Command.hpp"

void CommandNick::execute(IRCClient *client, const std::string &params)
{
    std::string oldNick = client->getNickname();
    std::stringstream paramsStream(params);
    std::string nick;
    paramsStream >> nick;
    std::cout << "nick: " << nick << std::endl;
    if (nick == ":")
    {
        client->sendMessages(":" + oldNick + " 431 " + oldNick + " :No nickname given");
        return;
    }
    client->setNickname(nick);
    client->sendMessages(":" + oldNick + " NICK " + nick);
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
    client->setPassword(params);
}

void CommandJoin::execute(IRCClient *client, const std::string &params)
{
    std::string channelName = params;
    if (server->getChannel(channelName) == nullptr)
    {
        server->createChannel(channelName);
    }
    IRCChannel *channel = server->getChannel(channelName);
    channel->addClient(client);
    client->sendMessages(":" + client->getNickname() + "!~" + client->getUsername() + "@" + client->getHostname() + " JOIN " + channelName);
}