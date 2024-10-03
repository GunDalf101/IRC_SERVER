#include "Command.hpp"

void CommandTopic::execute(IRCClient *client, const std::string &params)
{
    std::string channelName;
    std::string topic;
    std::stringstream paramsStream(params);
    paramsStream >> channelName;
    if (channelName.empty()) {
        client->sendMessages(ERR_NEEDMOREPARAMS(client->getNickname(), client->getHostname(), "TOPIC"));
        return;
    }
    std::getline(paramsStream, topic);

    if(!client->isAuthentificated())
        return client->sendMessages(ERR_NOTREGISTERED(client->getHostname(), client->getNickname()));

    size_t colonPos = topic.find(':');
    if (colonPos != std::string::npos) {
        topic = topic.substr(colonPos + 1);
    }
    IRCChannel *channel = server->getChannel(channelName);
    if (channel == NULL) {
        client->sendMessages(ERR_NOSUCHCHANNEL(client->getHostname(), client->getNickname(), channelName));
        return;
    }
    if (channel->getClient(client->getNickname()) == NULL) {
        client->sendMessages(ERR_NOTONCHANNEL(client->getHostname(), channelName));
        return;
    }
    if (topic.empty()) {
        if (channel->getTopic().empty()) 
            client->sendMessages(RPL_NOTOPIC(client->getHostname(), client->getNickname(), channelName));
        else
            client->sendMessages(RPL_TOPIC(client->getHostname(), client->getNickname(), channelName, channel->getTopic()));
        return;
    }
    if (!channel->getOperator(client->getNickname()) && channel->getModes(true).find("t") != std::string::npos) {
        client->sendMessages(ERR_CHANOPRIVSNEEDED(client->getHostname(), client->getNickname(), channelName));
        return;
    }
    channel->setTopic(topic);
    client->sendMessages(RPL_TOPIC(client->getHostname(), client->getNickname(), channelName, topic));
    channel->notifyClients(RPL_TOPIC(client->getHostname(), client->getNickname(), channelName, topic), client->getNickname());
}