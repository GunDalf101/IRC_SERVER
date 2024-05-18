#include "Command.hpp"
#include <iostream>

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void CommandJoin::checkChannel(std::unordered_map<std::string, std::string> channelKeyMap, IRCClient *client){
    (void)client;
    std::unordered_map<std::string, std::string>::iterator it;
    it = channelKeyMap.begin();
    if (server->getChannel(it->first) == NULL) {
        server->createChannel(it->first);
    }
    while (it != channelKeyMap.end()) {
        //check if channel start zith #
        if (it->first[0] != '#' || it->first[0] != '&') {
            client->sendMessages(ERR_BADCHANNELNAME(client->getNickname(), client->getHostname(), it->first));
        }
        if (it->first.length() > 50) {
            std::cout << "Invalid channel name" << std::endl;
            return;
        }
        // check if they contqin spaces control bell or comma
        // if (it->first.find(' ') != std::string::npos || it->first.find(',') != std::string::npos || it->first.find('\a') != std::string::npos) {
        //     std::cout << "Invalid channel name" << std::endl;
        //     return;
        // }
        it++;
    }
    std::cout << std::endl;
}

void CommandJoin::execute(IRCClient *client, const std::string &params)
{
    (void)client;
    std::vector<std::string> paramList = split(params, ' ');
    if (!paramList.empty()) {
        std::vector<std::string> channels = split(paramList[0], ',');
        std::vector<std::string> keys;
        if (paramList.size() > 1) {

            keys = split(paramList[1], ',');
        }
        std::unordered_map<std::string, std::string> channelKeyMap;
        for (size_t i = 0; i < channels.size(); ++i) {
            if (i < keys.size()) {
                channelKeyMap[channels[i]] = keys[i];
            } else {
                channelKeyMap[channels[i]] = "";
            }
        }
        checkChannel(channelKeyMap, client);
    }
    
}