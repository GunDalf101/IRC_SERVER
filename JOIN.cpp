#include "Command.hpp"
#include "CommandRpl.hpp"
#include <iostream>


// static std::vector<std::string> split(const std::string& s, char delimiter) {
//     std::vector<std::string> tokens;
//     std::string token;
//     std::istringstream tokenStream(s);
//     while (std::getline(tokenStream, token, delimiter)) {
//         tokens.push_back(token);
//     }
//     return tokens;
// }

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

void CommandJoin::handleChannel(std::map<std::string, std::string> channelKeyMap, IRCClient *client){
    (void)client;
    std::map<std::string, std::string>::iterator it;
    it = channelKeyMap.begin();
    while (it != channelKeyMap.end()) {
        if (it->first[0] != '#' || it->first.length() > 50){
            client->sendMessages(ERR_BADCHANNELNAME(client->getNickname(), client->getHostname(), it->first));
            it++;
            continue;
        }
        if (server->getChannel(it->first)) {
            IRCChannel *channel = server->getChannel(it->first);
            if (channel->getKey() != it->second){
                client->sendMessages(ERR_BADCHANNELKEY(client->getNickname(), client->getHostname(), it->first));
                it++;
                continue;
            }
            if (channel->getClient(client->getNickname())) {
                client->sendMessages(ERR_USERONCHANNEL(client->getHostname(), client->getNickname(), client->getNickname(), it->first));
                it++;
                continue;
            }
            channel->addUser(client);
        }

        if (!server->getChannel(it->first)) {
            server->createChannel(it->first);
            IRCChannel *channel = server->getChannel(it->first);
            channel->addOperator(client);
            channel->setKey(it->second);
        }

        IRCChannel *channel = server->getChannel(it->first);

        if (channel->hasUserLimit() && channel->getNumUsers() >= channel->getLimit()) {
            client->sendMessages(ERR_CHANNELISFULL(client->getNickname(), client->getHostname(), it->first));
            it++;
            continue;
        }

        client->sendMessages(RPL_JOIN(client->getNickname(), client->getUsername(), it->first, client->getIpAddr()));
        channel->notifyClients(RPL_JOIN(client->getNickname(), client->getUsername(), it->first, client->getIpAddr()), client->getNickname());
        if (!channel->getTopic().empty()) {
            client->sendMessages(RPL_TOPIC(client->getHostname(), client->getNickname(), it->first, channel->getTopic()));
        }
        sendNameList(client, channel);
        it++;
    }
    std::cout << std::endl;
}

void CommandJoin::execute(IRCClient *client, const std::string &params)
{
    std::vector<std::string> paramList = split(params, ' ');
    if (!paramList.empty()) {
        std::vector<std::string> channels = split(paramList[0], ',');
        std::vector<std::string> keys;
        if (paramList.size() > 1)
            keys = split(paramList[1], ',');
        std::map<std::string, std::string> channelKeyMap;
        for (size_t i = 0; i < channels.size(); ++i) {
            if (i < keys.size()) {
                channelKeyMap[channels[i]] = keys[i];
            } else {
                channelKeyMap[channels[i]] = "";
            }
        }
        if (channelKeyMap.empty()) {
            client->sendMessages(ERR_NEEDMOREPARAMS(client->getNickname(), client->getHostname(), "JOIN"));
            return;
        }
        handleChannel(channelKeyMap, client);
    }
}
