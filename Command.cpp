#include "Command.hpp"

void CommandNick::execute(IRCClient *client, const std::string &params)
{
    std::string oldNick = client->getNickname();
    std::stringstream paramsStream(params);
    std::string nick;
    paramsStream >> nick;
    client->setNickname(nick);
    client->sendMessages(":" + oldNick + " NICK " + nick);
}

void CommandUser::execute(IRCClient *client, const std::string &params)
{
    client->setUsername(params);
}

void CommandPass::execute(IRCClient *client, const std::string &params)
{
    client->setPassword(params);
}

void CommandJoin::execute(IRCClient *client, const std::string &params)
{
    client->sendMessages("JOIN " + params);
}