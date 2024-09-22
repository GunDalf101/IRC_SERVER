#include "Command.hpp"
#include "CommandRpl.hpp"

void CommandMode::execute(IRCClient *client, const std::string &params)
{
    std::vector<std::string> paramArr = toReqArgs(params);

    if (client->isAuthentificated())
    {
        if (paramArr.size())
        {
            std::string target = paramArr[0];
            if (!server.getChannel(target))
            {
                client->sendMessages(ERR_NOSUCHCHANNEL(server.getHostName(), client->getNickname(), target));
                return;
            }
            if (paramArr.size() == 1)
            {
                replyWithTargetState(client, target);
            }
            else
            {
                if (!server.getChannel(target)->getOperator(client->getNickname()))
                {
                    client->sendMessages(ERR_CHANOPRIVSNEEDED(server.getHostName(), client->getNickname(), target));
                    return;
                }
            }
        }
        else
        {
            client->sendMessages(ERR_NEEDMOREPARAMS(server.getHostName(), client->getNickname(), name));
        }
    }
    else
    {
        client->sendMessages(ERR_NOTREGISTERED(server.getHostName(), client->getNickname()));
    }
}

void CommandMode::replyWithTargetState(IRCClient *client, std::string target)
{
    IRCChannel &ch = *(server.getChannel(target));
    std::string modes;

    modes += '+';
    if (ch.hasUserLimit())
        modes += 'l';
    if (ch.isInviteOnly())
        modes += 'i';
    if (ch.hasTopicProtection())
        modes += 't';
    if (ch.hasUserLimit())
        modes.append(' ' + intToString(ch.getLimit()));
    client->sendMessages(RPL_CHANNELMODEIS(server.getHostName(), client->getNickname(), target, modes));
    client->sendMessages(RPL_CREATIONTIME(server.getHostName(), client->getNickname(), target, ch.getCreationTime()));
}

std::string CommandMode::intToString(int i)
{
    std::ostringstream oss;
    
    oss << i;
    return oss.str();   
}