#include "Command.hpp"
#include "CommandRpl.hpp"

static bool isModefier(char c)
{
    return (c == '+' || c == '-');
}

static bool isTypeB(char c)
{
    return (c == 'o');
}

static bool isTypeC(char c)
{
    return (c == 'k' || c == 'l');
}

static bool isTypeD(char c)
{
    return (c == 'i' || c == 't');
}

static bool isKnownMod(char c)
{
    return (isTypeB(c) || isTypeC(c) || isTypeD(c));
}

static bool isValidKey(std::string key)
{
    uint i = 0;

    if (key[0] == ':')
        return (false);
    while (i < key.size())
    {
        if (key[i] == ' ')
            return (false);
        i++;
    }
    return (true);
}

static void addResultMode(bool set, std::string& resultMode, char c)
{
    char setAsC = set ? '+' : '-';
    if (resultMode.length() == 0)
        resultMode += setAsC;
    else
    {
        uint i = resultMode.length() - 1;
        while (i >= 0)
        {
            if (isModefier(resultMode[i]))
            {
                if (resultMode[i] != setAsC)
                    resultMode += setAsC;
                else
                    break;
            }   
            i--;
        }
    }
    resultMode += c;
}

static bool isValidInt(std::string s)
{
    uint i = 0;
    
    while (i < s.size())
    {
        if (s[i] < '0' || s[i] > '9')
            if (s[i] != '+' && s[i] != '-')
                return (false);
        i++;
    }
    return (true);
}

static std::string buildResultModeWithArgs(std::string resultMode, std::vector<std::string> resultArgs)
{
    std::ostringstream joined;

    joined << resultMode;
    if (resultArgs.empty())
        return (joined.str());
    for (std::vector<std::string>::iterator i = resultArgs.begin(); i != resultArgs.end(); i++)
        joined << " " << *i;
    return (joined.str());
}

void CommandMode::execute(IRCClient *client, const std::string &params)
{
    std::vector<std::string> paramArr = toReqArgs(params);

    if (client->isAuthentificated())
    {
        if (paramArr.size())
        {
            std::string channel = paramArr[0];
            IRCChannel *chp = server.getChannel(channel);
            if (!chp)
            {
                client->sendMessages(ERR_NOSUCHCHANNEL(server.getHostName(), client->getNickname(), channel));
                return;
            }
            if (paramArr.size() == 1)
            {
                replyWithChannelState(client, channel, true);
            }
            else
            {
                IRCChannel &ch = *chp;
                if (!ch.getOperator(client->getNickname()))
                {
                    client->sendMessages(ERR_CHANOPRIVSNEEDED(server.getHostName(), client->getNickname(), channel));
                    return;
                }
                std::string modes = paramArr[1];
                paramArr.erase(paramArr.begin(), paramArr.begin() + 2);
                std::reverse(paramArr.begin(),paramArr.end());
                uint i = 0;
                enum Mod { Default, Set, Unset } mod;
                std::string resultMode;
                std::vector<std::string> resultArgs;
                while (i < modes.size())
                {
                    if (isModefier(modes[i]))
                    {
                        mod = (modes[i] == '+' ? Set : Unset);
                        i++;
                        continue;
                    }
                    if (isKnownMod(modes[i]))
                    {
                        if ((isTypeB(modes[i]) || (isTypeC(modes[i]) && (mod == Set))) && paramArr.empty())
                        {
                            i++;
                            continue;
                        }
                        else if (isTypeB(modes[i]))
                        {
                            std::string nick = paramArr.back();
                            paramArr.pop_back();
                            if (server.getClientByNickname(nick))
                            {

                                if (ch.isMember(nick) || ch.isOp(nick))
                                {
                                    if (mod == Set)
                                    {
                                        if (!ch.isOp(nick))
                                        {
                                            IRCClient *c = ch.getMember(nick);
                                            ch.promoteToOp(c);
                                            resultArgs.push_back(nick);
                                            addResultMode(true, resultMode, modes[i]);
                                        }
                                    }
                                    else if (mod == Unset)
                                    {
                                        if (ch.isOp(nick))
                                        {
                                            IRCClient *c = ch.getOperator(nick);
                                            ch.demoteFromOp(c);
                                            resultArgs.push_back(nick);
                                            addResultMode(false, resultMode, modes[i]);
                                        } 
                                    }
                                }
                                else
                                {
                                    client->sendMessages(ERR_USERNOTINCHANNEL(server.getHostName(), client->getNickname(), channel));
                                }
                            }
                            else
                            {
                                client->sendMessages(ERR_NOSUCHNICK(server.getHostName(), client->getNickname(), nick));
                            }
                        }
                        else if (isTypeC(modes[i]))
                        {
                            if (mod == Set)
                            {
                                std::string arg = paramArr.back();
                                paramArr.pop_back();
                                if (modes[i] == 'k')
                                {
                                    if (!arg.size() || !isValidKey(arg))
                                        client->sendMessages(ERR_INVALIDKEY(server.getHostName(), client->getNickname(), ch.getName()));
                                    else if (!ch.hasKey())
                                    {
                                        ch.setKey(arg);
                                        resultArgs.push_back(arg);
                                        addResultMode(true, resultMode, modes[i]);
                                    }
                                }
                                else
                                {
                                    if (isValidInt(arg))
                                    {
                                        ch.setLimit(std::stoi(arg) <= 0 ? -1 : std::stoi(arg));
                                        resultArgs.push_back(arg);
                                        addResultMode(true, resultMode, modes[i]);
                                    }
                                    else
                                    {
                                        
                                    }
                                }
                            }
                            else if (mod == Unset)
                            {
                                if (modes[i] == 'k' && ch.hasKey())
                                {
                                    ch.unsetKey();
                                    addResultMode(false, resultMode, modes[i]);
                                }
                                else if (modes[i] == 'l' && ch.hasUserLimit())
                                {
                                    ch.setLimit(-1);
                                    addResultMode(false, resultMode, modes[i]);
                                }
                            }
                        }
                        else if (isTypeD(modes[i]))
                        {
                            if (mod == Set)
                            {
                                if (modes[i] == 'i' && !ch.isInviteOnly())
                                {
                                    ch.inviteOnly();
                                    addResultMode(true, resultMode, modes[i]);
                                }
                                else if (modes[i] == 't' && !ch.hasTopicProtection())
                                {
                                    ch.setTopicProtection();
                                    addResultMode(true, resultMode, modes[i]);
                                }
                            }
                            else if (mod == Unset)
                            {
                                if (modes[i] == 'i' && ch.isInviteOnly())
                                {
                                    ch.unsetInviteOnly();
                                    addResultMode(false, resultMode, modes[i]);
                                }
                                else if (modes[i] == 't' && ch.hasTopicProtection())
                                {
                                    ch.unsetTopicProtection();
                                    addResultMode(false, resultMode, modes[i]);
                                }
                            }
                        }
                    }
                    else
                    {
                        client->sendMessages(ERR_UNKNOWNMODE(server.getHostName(), client->getNickname(), modes[i]));
                    }
                    i++;
                }
                ch.notifyMembers(RPL_MODE(client->getNickname(), client->getUsername(), server.getHostName(), ch.getName(), buildResultModeWithArgs(resultMode, resultArgs)));
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

void CommandMode::replyWithChannelState(IRCClient *client, std::string channel, bool isOp)
{
    IRCChannel &ch = *(server.getChannel(channel));

    client->sendMessages(RPL_CHANNELMODEIS(server.getHostName(), client->getNickname(), channel, ch.getModes(isOp)));
    client->sendMessages(RPL_CREATIONTIME(server.getHostName(), client->getNickname(), channel, ch.getCreationTime()));
}
