#include "Command.hpp"
#include <iostream>
#include <string>

std::string getColonArg(std::string arg)
{
    if(!arg.empty() && arg.at(0) == ':')
        arg.erase(0,1);
    return arg;
}

bool isValidChar(char c)
{
    std::string special = "\\[]{}|";
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
    if(isdigit(nickname.at(0)))
        return false;
    for(unsigned int i = 0; i < nickname.length(); i++)
        if(!isValidChar(nickname.at(i)))
            return false;
    return true;
}

void CommandNick::execute(IRCClient *client, const std::string &params)
{
    if((client->authLevel & 1) == 0)
        return;
    std::string oldNick = client->getNickname();
    std::string nick;
    std::vector<std::string> args = toReqArgs(params);
    if (args.empty())
        return client->sendMessages(ERR_NONICKNAMEGIVEN(nick, client->getHostname()));

    nick = args.at(0);
    if (!isValidNickUser(nick))
        client->sendMessages(ERR_ERRONEUSNICKNAME(nick, client->getHostname()));
    else if(isAlreadyInuse(nick))
        client->sendMessages(ERR_NICKNAMEINUSE(nick, client->getHostname()));
    else
    {
        client->setNickname(nick);
        if(client->isAuthentificated())
        {
            client->sendMessages(NICKNAME_RPLY(oldNick, client->getUsername(), client->getHostname(), nick));
            server.broadcastToChannels(client->getNickname(), NICKNAME_RPLY(oldNick, client->getUsername(), client->getHostname(), nick));
        }
        else
        {
            client->authLevel |= 2;
            server.welcome_client(client);
        }
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

    if((client->authLevel & 1) == 0)
        return;
    if(client->isAuthentificated())
        return client->sendMessages(ERR_ALREADYREGISTERED(client->getNickname(), client->getHostname()));

    std::vector<std::string> args = toReqArgs(params);

    if (args.size() < 4 || args[3].empty())
        return client->sendMessages(ERR_NEEDMOREPARAMS(client->getNickname(), client->getHostname(), "USER"));

    username = args[0];
    mode = args[1];
    unused = args[2];
    realname = args[3];
    client->setUsername(username);
    client->setRealname(realname);

    client->authLevel |= 4;
    server.welcome_client(client);
}

void CommandPass::execute(IRCClient *client, const std::string &params)
{
    if(client->isAuthentificated())
        client->sendMessages(ERR_ALREADYREGISTERED(client->getNickname(), client->getHostname()));
    else if(client->authLevel&1)
        return;
    else if(params.empty())
        client->sendMessages(ERR_NEEDMOREPARAMS(client->getNickname(), client->getHostname(), "PASS"));
    else if(params.compare(server.getPassword()))
        client->sendMessages(ERR_PASSWDMISMATCH(client->getNickname(), client->getHostname()));
    else
        client->authLevel |= 1;
}

void CommandPart::execute(IRCClient *client, const std::string &params)
{

    if(!client->isAuthentificated())
        return client->sendMessages(ERR_NOTREGISTERED(client->getNickname(), client->getHostname()));
    std::vector<std::string> args = toReqArgs(params);
    if (args.size() == 0)
        return client->sendMessages(ERR_NEEDMOREPARAMS(client->getNickname(), client->getHostname(), "PART"));

    std::string reason;
    if (args.size() > 1)
        reason = args.at(1);
    std::vector<std::string> channels = split(args.at(0), ',');
    for(size_t i = 0; i < channels.size(); i++)
    {
        std::string channelName = channels.at(i);
        IRCChannel *channel = server->getChannel(channelName);
        if (channel == NULL)
            client->sendMessages(ERR_NOSUCHCHANNEL(client->getHostname(), client->getNickname(), channelName));
        else
        {
            if(!channel->isClientExists(client->getNickname()))
                client->sendMessages(ERR_NOTONCHANNEL(client->getHostname(), channelName));
            else
            {
                channel->notifyClients(RPL_NOTIFYPART(client->getNickname(), client->getHostname(), channelName, reason), client->getNickname());
                client->sendMessages(RPL_NOTIFYPART(client->getNickname(), client->getHostname(), channelName, reason));
                channel->removeMember(client);
                if(channel->getNumUsers() == 0)
                    server->removeChannel(channelName);
            }
        }
    }
}

void CommandKick::execute(IRCClient *client, const std::string &params)
{
    std::string reason;
    // bool isGivenReason = false;
    std::string channelName;
    IRCChannel *channel;
    IRCClient  *target;

    if(!client->isAuthentificated())
        return client->sendMessages(ERR_NOTREGISTERED(client->getNickname(), client->getHostname()));

    std::vector<std::string> args = toReqArgs(params);

    if (args.size() < 2)
        return client->sendMessages(ERR_NEEDMOREPARAMS(client->getNickname(), client->getNickname(), "KICK"));
    
    std::vector<std::string> targets = split(args.at(1), ',');
    if(args.size() > 2)
        reason = args[2];

    channelName = args[0];
    channel = this->server->getChannel(channelName);
    if(channel == NULL)
        return client->sendMessages(ERR_NOSUCHCHANNEL(client->getHostname(), client->getNickname(), channelName));
    if(!channel->isClientExists(client->getNickname()))
        return client->sendMessages(ERR_NOTONCHANNEL(client->getHostname(), channelName));
    if(!channel->isOp(client->getNickname()))
        return client->sendMessages(ERR_CHANOPRIVSNEEDED(client->getHostname(), client->getNickname(), channelName));
    for (size_t i = 0; i < targets.size(); i++)
    {
        target = channel->getClient(targets[i]);
        if(target == NULL)
        {
            client->sendMessages(ERR_USERNOTINCHANNEL(client->getHostname(), client->getNickname(), channelName));
            continue;
        }
        channel->notifyClients(RPL_KICK(client->getNickname(), client->getUsername(), client->getHostname(), target->getNickname() ,channelName, reason), client->getNickname());
        client->sendMessages(RPL_KICK(client->getNickname(),client->getUsername(), client->getHostname(), target->getNickname(), channelName, reason));
        channel->removeMember(target);
        if(channel->getNumUsers() == 0)
            server->removeChannel(channelName);

        if(target == client)
            break;
    }
}

void CommandPrivMsg::execute(IRCClient *client, const std::string &params)
{
    std::string target;
    std::string message;
    IRCChannel *rcvChannel;
    IRCClient  *rcvClient;

    if(!client->isAuthentificated())
        return client->sendMessages(ERR_NOTREGISTERED(client->getNickname(), client->getHostname()));

    std::vector<std::string> args = toReqArgs(params);

    if (args.size() == 0)
        return client->sendMessages(ERR_NORECIPIENT(client->getHostname(), client->getNickname(), "PRIVMSG"));
    if (args.size() == 1 || args.at(1).empty())
        return client->sendMessages(ERR_NOTEXTTOSEND(client->getNickname(), client->getHostname()));

    std::vector<std::string> targets = split(args.at(0), ',');

    message = args.at(1);
    for(size_t i = 0; i < targets.size(); i++)
    {
        target = targets.at(i);
        if(target.at(0) == '#')
        {
            rcvChannel = this->server.getChannel(target);
            if(rcvChannel == NULL)
                client->sendMessages(ERR_NOSUCHCHANNEL(client->getHostname(), client->getNickname(), target));
            else if(!rcvChannel->isClientExists(client->getNickname()))
                client->sendMessages(ERR_CANNOTSENDTOCHAN(client->getHostname(), client->getNickname(), target));
            else
                rcvChannel->notifyClients(PRIVMSG_FORMAT(client->getNickname(), client->getUsername(), client->getHostname(), target, message), client->getNickname());
        } else {
            rcvClient = server.getClientByNickname(target);
            if(rcvClient == NULL || !rcvClient->isAuthentificated())
                client->sendMessages(ERR_NOSUCHNICK(client->getHostname(), client->getNickname(), target));
            else
                rcvClient->sendMessages(PRIVMSG_FORMAT(client->getNickname(), client->getUsername(), client->getHostname(), rcvClient->getNickname(), message));
        }
    }
}
