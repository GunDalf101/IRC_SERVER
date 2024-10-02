#include "Command.hpp"
#include "CommandRpl.hpp"
#include <iostream>

static std::vector<std::string> splitHere(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

bool CommandJoin::sendNameList(IRCClient *client, IRCChannel *channel){
    std::string clients;
    std::vector<IRCClient *> members = channel->getClients();
    for (std::vector<IRCClient *>::iterator it = members.begin(); it != members.end(); it++) {
        clients += (*it)->getNickname() + " ";
    }
    members = channel->getOperators();
    for (std::vector<IRCClient *>::iterator it = members.begin(); it != members.end(); it++) {
        clients += "@" + (*it)->getNickname() + " ";
    }
    client->sendMessages(RPL_NAMREPLY(client->getHostname(), clients, channel->getName(), client->getNickname()));
    client->sendMessages(RPL_ENDOFNAMES(client->getHostname(), client->getNickname(), channel->getName()));
    return true;
}

void CommandJoin::handleChannel(std::vector<std::pair<std::string, std::string> > channelKeyMap, IRCClient *client){
    (void)client;
    std::vector<std::pair<std::string, std::string> >::iterator it;
    it = channelKeyMap.begin();
    while (it != channelKeyMap.end()) {
        if (it->first[0] != '#' || it->first.length() > 50){
            client->sendMessages(ERR_BADCHANNELNAME(client->getNickname(), client->getHostname(), it->first));
            it++;
            continue;
        }
        if (server->getChannel(it->first)) {
            IRCChannel *channel = server->getChannel(it->first);
            std::cout << "Channel mode: " << channel->getModes(true) << std::endl;
            if (channel->getClient(client->getNickname())) {
                client->sendMessages(ERR_USERONCHANNEL(client->getHostname(), client->getNickname(), client->getNickname(), it->first));
                it++;
                continue;
            }
            if (channel->hasUserLimit() && channel->getNumUsers() >= channel->getLimit()) {
                client->sendMessages(ERR_CHANNELISFULL(client->getHostname(), client->getNickname(), it->first));
                it++;
                continue;
            }
            if (channel->getKey() != it->second  && channel->getModes(true).find("k") != std::string::npos && !client->isInvited(channel->getName())){
                client->sendMessages(ERR_BADCHANNELKEY(client->getNickname(), client->getHostname(), it->first));
                it++;
                continue;
            }
            if (channel->isInviteOnly() && !client->isInvited(channel->getName())) {
                client->sendMessages(ERR_INVITEONLY(client->getHostname(), client->getNickname(), it->first));
                it++;
                continue;
            }
            if (client->isInvited(channel->getName())) 
                client->removeInvite(channel->getName());
            channel->addMember(client);
        }

        if (!server->getChannel(it->first)) {
            server->createChannel(it->first);
            IRCChannel *channel = server->getChannel(it->first);
            channel->addOperator(client);
            if (!it->second.empty())
                channel->setKey(it->second);
        }

        IRCChannel *channel = server->getChannel(it->first);

        client->sendMessages(RPL_JOIN(client->getNickname(), client->getUsername(), it->first, client->getIpAddr()));
        channel->notifyClients(RPL_JOIN(client->getNickname(), client->getUsername(), it->first, client->getIpAddr()), client->getNickname());
        if (!channel->getTopic().empty()) {
            client->sendMessages(RPL_TOPIC(client->getHostname(), client->getNickname(), it->first, channel->getTopic()));
        }
        sendNameList(client, channel);
        it++;
    }
}

static std::string getChannelsOfMember(std::map<std::string, IRCChannel*> channels, IRCClient* client)
{
    IRCChannel *c;
    std::string result = "";
    for (std::map<std::string, IRCChannel*>::iterator i = channels.begin(); i != channels.end(); i++)
    {
        c = i->second;
        if (c->isClientExists(client->getNickname()))
        {
            result.append(c->getName() + ",");
        }
    }
    return (result);
}

void CommandJoin::execute(IRCClient *client, const std::string &params)
{
    if(!client->isAuthentificated())
        return client->sendMessages(ERR_NOTREGISTERED(client->getHostname(), client->getNickname()));

    std::vector<std::string> paramList = split(params, ' ');

    for (size_t i = 0; i < paramList.size(); i++) {
        if (paramList[i][0] == ':')
            paramList[i] = paramList[i].substr(1, paramList[i].length());
    }

    if (!paramList.empty()) {
        if (paramList.size() == 1 && paramList[0] == "0")
        {
            std::string channels = getChannelsOfMember(server->getChannels(), client);
            if (channels.empty())
                return;
            else
                channels.append(" :Left all channels");
            CommandPart(server).execute(client, channels);
            return;
        }
        std::vector<std::string> channels = splitHere(paramList[0], ',');
        std::vector<std::string> keys;
        if (paramList.size() > 1)
            keys = splitHere(paramList[1], ',');
        std::vector<std::pair<std::string, std::string> > channelKeyMap;
        for (size_t i = 0; i < channels.size(); i++) {
            if (keys.size() > i)
                channelKeyMap.push_back(std::pair<std::string, std::string>(channels[i], keys[i]));
            else
                channelKeyMap.push_back(std::pair<std::string, std::string>(channels[i], ""));
        }
        handleChannel(channelKeyMap, client);
    } else {
        client->sendMessages(ERR_NEEDMOREPARAMS(client->getNickname(), client->getHostname(), "JOIN"));
    }

}
