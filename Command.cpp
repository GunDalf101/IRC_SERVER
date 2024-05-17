#include "Command.hpp"
#include <iostream>

void CommandNick::execute(IRCClient *client, const std::string &params)
{
    std::string oldNick = client->getNickname();
    std::stringstream paramsStream(params);
    std::string nick;
    paramsStream >> nick;
    // std::cout << "nick: " << nick << std::endl;
    if (nick == ":")
    {
        client->sendMessages(ERR_NONICKNAMEGIVEN(nick, client->getHostname()));
        return;
    }
    if(isAlreadyInuse(params))
    {
        client->sendMessages(ERR_NICKNAMEINUSE(params, client->getHostname()));
        return ;
    }

    client->setNickname(nick);
    // std::cout << client->getUsername().empty() << std::endl;
    // if(client->getUsername().empty())
    //     client->setUsername(nick);
    client->sendMessages(NICKNAME_RPLY(oldNick, client->getUsername(), client->getHostname(), nick));
}

// bool CommandNick::isValidNick(const std::string &nickname)
// {

// }

bool CommandNick::isAlreadyInuse(const std::string &new_nickname)
{
    std::unordered_map<int, IRCClient*> clients = server.getCliens();
    std::unordered_map<int, IRCClient*>::iterator i = clients.begin();
    while (i != clients.end())
    {
        if (!(*i).second->getNickname().compare(new_nickname))
            return true;
        i++;
    }
    return false;
}

void CommandUser::execute(IRCClient *client, const std::string &params)
{
    std::string username;
    std::string mode;
    std::string unused;
    std::string realname;
    std::stringstream paramsStream(params);
    paramsStream >> username >> mode >> unused;
    std::getline(paramsStream, realname);
    if(realname.length() < 2)
        return ;
    realname.erase(0,1);
    if(realname.at(0) == ':')
        realname.erase(0, 1);
    else
        if(realname.find(' ') != std::string::npos)
            return ;
    std::cout << "[" +username+ "]" << "[" +mode+ "]" << "[" +unused+ "]" << "[" +realname+ "]" << std::endl;
    client->setNickname(username);
    client->setUsername(username);
    client->setRealname(realname);
}

void CommandPass::execute(IRCClient *client, const std::string &params)
{
    if(params.compare(server.getPassword()))
        client->sendMessages(ERR_PASSWDMISMATCH(client->getNickname(), client->getHostname()));
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
    client->sendMessages(RPL_JOIN(client->getNickname(), client->getUsername(), channelName, client->getIpAddr()));
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