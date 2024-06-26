#include "Command.hpp"
#include <iostream>

std::string getColonArg(std::string arg)
{
    if(!arg.empty() && arg.at(0) == ':')
        arg.erase(0,1);
    return arg;
}

bool isValidChar(char c)
{
    std::string special = "-_\\[]{}^|";
    if(isalnum(c))
        return true;
    for(unsigned int i = 0; i < special.length(); i++)
        if(special.at(i) == c)
            return true;
    return false;
}

bool CommandNick::isValidNickUser(const std::string &nickname)
{
    if(nickname.length() > 30)
        return false;
    if(!isalpha(nickname.at(0)))
        return false;
    for(unsigned int i = 0; i < nickname.length(); i++)
        if(!isValidChar(nickname.at(i)))
            return false;
    return true;
}

void CommandNick::execute(IRCClient *client, const std::string &params)
{
    std::string oldNick = client->getNickname();
    std::string nick;
    nick = getColonArg(params);
    if (nick.empty())
        client->sendMessages(ERR_NONICKNAMEGIVEN(nick, client->getHostname()));
    else if (!isValidNickUser(nick))
        client->sendMessages(ERR_ERRONEUSNICKNAME(nick, client->getHostname()));
    else if(isAlreadyInuse(nick))
        client->sendMessages(ERR_NICKNAMEINUSE(nick, client->getHostname()));
    else
    {
        client->setNickname(nick);
        client->sendMessages(NICKNAME_RPLY(oldNick, client->getUsername(), client->getHostname(), nick));
    }
}

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
    client->setNickname(username);
    client->setUsername(username);
    client->setRealname(realname);
}

void CommandPass::execute(IRCClient *client, const std::string &params)
{
    if(params.compare(server.getPassword()))
        client->sendMessages(ERR_PASSWDMISMATCH(client->getNickname(), client->getHostname()));
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
    channel->removeUser(client);
    client->sendMessages(":" + client->getNickname() + "!~" + client->getUsername() + "@" + client->getHostname() + " PART " + channelName);
}

void CommandInvite::execute(IRCClient *client, const std::string &params)
{
    std::string nickname;
    std::string channelName;
    std::stringstream paramsStream(params);
    paramsStream >> nickname >> channelName;
    IRCChannel *channel = server->getChannel(channelName);
    if (channel == NULL)
    {
        client->sendMessages(ERR_NOSUCHCHANNEL(client->getNickname(), client->getHostname(), channelName));
        return;
    }
    IRCClient *invitedClient = server->getClientByNickname(nickname);
    if (invitedClient == NULL)
    {
        client->sendMessages(ERR_NOSUCHNICK(client->getNickname(), client->getHostname(), nickname));
        return;
    }
    if (!channel->getOperator(client->getNickname()))
    {
        client->sendMessages(ERR_CHANOPRIVSNEEDED(client->getNickname(), client->getHostname(), channelName));
        return;
    }
    if (channel->getClient(invitedClient->getNickname()))
    {
        client->sendMessages(ERR_USERONCHANNEL(client->getHostname(), client->getNickname(), invitedClient->getNickname(), channelName));
        return;
    }
    invitedClient->invite(channelName);
    client->sendMessages(RPL_INVITING(client->getNickname(), client->getHostname(), invitedClient->getNickname(), channelName));
    invitedClient->sendMessages(RPL_INVITE(client->getNickname(), invitedClient->getUsername(), invitedClient->getHostname(), channelName));
    
}

void CommandKick::execute(IRCClient *client, const std::string &params)
{
    (void)client;
    (void)params;
    
}
