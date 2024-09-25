#include "Command.hpp"
#include <iostream>
#include <string>

std::vector<std::string> toReqArgs(const std::string &params)
{
    size_t index = params.find(':');
    std::string co = params;
    if(index != std::string::npos)
        co = params.substr(0, index);

    std::vector<std::string> substrings;
    std::stringstream ss(co);
    std::string param;
    
    while (std::getline(ss, param, ' '))
        if(!param.empty())
            substrings.push_back(param);

    if (index != std::string::npos)
        substrings.push_back(params.substr(index + 1, params.length() - index - 1));

    return substrings;
}

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
        if(special.at(i)  == c)
            return true;
    return false;
}

bool CommandNick::isValidNickUser(const std::string &nickname)
{
    if (nickname.empty())
        return false;
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
    if(client->authLevel != 1)
        return;

    std::string oldNick = client->getNickname();
    std::string nick;
    std::vector<std::string> args = toReqArgs(params);
    if (args.empty())
    {
        client->sendMessages(ERR_NONICKNAMEGIVEN(nick, client->getHostname()));
        return;
    }
    nick = args.at(0);
    if (!isValidNickUser(nick))
        client->sendMessages(ERR_ERRONEUSNICKNAME(nick, client->getHostname()));
    else if(isAlreadyInuse(nick))
        client->sendMessages(ERR_NICKNAMEINUSE(nick, client->getHostname()));
    else
    {
        client->setNickname(nick);
        client->sendMessages(NICKNAME_RPLY(oldNick, client->getUsername(), client->getHostname(), nick));
        client->authLevel = 2;
    }
}

bool CommandNick::isAlreadyInuse(const std::string &new_nickname)
{
    std::map<int, IRCClient*> clients = server.getCliens();
    std::map<int, IRCClient*>::iterator i = clients.begin();
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

    if(client->authLevel != 2)
        return;

    std::vector<std::string> args = toReqArgs(params);
    if (args.size() < 4)
    {
        client->sendMessages(ERR_NEEDMOREPARAMS(client->getNickname(), client->getHostname(), "USER"));
        return;
    }
    username = args[0];
    mode = args[1];
    unused = args[2];
    realname = args[3];
    // client->setNickname(username);
    client->setUsername(username);
    client->setRealname(realname);

    client->authLevel = 3;
    client->sendMessages(RPL_WELCOME(client->getNickname(), client->getHostname()));
    client->sendMessages(RPL_YOURHOST(client->getNickname(), client->getHostname()));
    client->sendMessages(RPL_CREATED(client->getNickname(), client->getHostname()));
    client->sendMessages(RPL_MYINFO(client->getNickname(), client->getHostname()));
}

void CommandPass::execute(IRCClient *client, const std::string &params)
{
    if(client->isAuthentificated())
        client->sendMessages(ERR_ALREADYREGISTERED(client->getNickname(), client->getHostname()));
    else if(params.empty())
        client->sendMessages(ERR_NEEDMOREPARAMS(client->getNickname(), client->getHostname(), "PASS"));
    else if(params.compare(server.getPassword()))
        client->sendMessages(ERR_PASSWDMISMATCH(client->getNickname(), client->getHostname()));
    else
        client->authLevel = 1;
}

void CommandPart::execute(IRCClient *client, const std::string &params)
{
    std::vector<std::string> args = toReqArgs(params);
    std::cout << "[" << args.size() << "]" << std::endl;
    if (args.size() == 0)
        return client->sendMessages(ERR_NEEDMOREPARAMS(client->getNickname(), client->getHostname(), "PART"));

    std::vector<std::string> channels = split(args.at(0), ',');
    for(size_t i = 0; i < channels.size(); i++)
    {
        std::string channelName = channels.at(i);
        IRCChannel *channel = server->getChannel(channelName);
        if (channel == NULL)
            client->sendMessages(":" + client->getNickname() + " 403 " + client->getNickname() + " " + channelName + " :No such channel");
        else
        {
            if(channel->isClientExists(client->getNickname()))
            {
                channel->removeMember(client);
                client->sendMessages(":" + client->getNickname() + "!~" + client->getUsername() + "@" + client->getHostname() + " PART " + channelName);
            } else
                client->sendMessages(ERR_NOTONCHANNEL(client->getHostname(), channelName));
        }
    }
}

void CommandKick::execute(IRCClient *client, const std::string &params)
{
    (void)client;
    (void)params;
}

void CommandPrivMsg::execute(IRCClient *client, const std::string &params)
{
    std::string target;
    std::string message;
    IRCChannel *rcvChannel;
    IRCClient  *rcvClient;

    std::vector<std::string> args = toReqArgs(params);

    if (args.size() < 2)
        return client->sendMessages(ERR_NEEDMOREPARAMS(client->getNickname(), client->getHostname(), "PRIVMSG"));
    if (args.at(1).empty())
        return client->sendMessages(ERR_NOTEXTTOSEND(client->getNickname(), client->getHostname()));

    std::vector<std::string> targets = split(args.at(0), ',');

    message = args.at(1);
    for(size_t i = 0; i < targets.size(); i++)
    {
        target = targets.at(i);
        if(target.at(0) == '#')
        {
            std::cout << '[' << target << ']' << std::endl;
            rcvChannel = this->server.getChannel(target);
            if(rcvChannel == NULL)
                client->sendMessages(ERR_NOSUCHCHANNEL(client->getHostname(), client->getNickname(), target));
            else if(!rcvChannel->isClientExists(client->getNickname()))
                client->sendMessages(ERR_CANNOTSENDTOCHAN(client->getHostname(), client->getNickname(), target));
            else
                rcvChannel->notifyClients(PRIVMSG_FORMAT(client->getNickname(), client->getUsername(), client->getHostname(), target, message), client->getNickname());
        } else {
            rcvClient = server.getClientByNickname(target);
            if(rcvClient == NULL)
                client->sendMessages(ERR_NOSUCHNICK(client->getHostname(), client->getNickname(), target));
            else
                rcvClient->sendMessages(PRIVMSG_FORMAT(client->getNickname(), client->getUsername(), client->getHostname(), rcvClient->getNickname(), message));
        }
    }
}

// multiple targets
// empty text
// need more params