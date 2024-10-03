#include "Channel.hpp"
#include "Client.hpp"

IRCChannel::IRCChannel(std::string name) : name(name)
{
    topic = "";
    type = "";
    key = "";
    userLimit = 0;
    setCreationTime();
    _inviteOnly = false;
    _hasKey = false;
    _hasTopicProtection = false;
    userLimit = -1;
}

IRCChannel::~IRCChannel()
{
}

void IRCChannel::addMember(IRCClient *client)
{
    users.push_back(client);
    members.push_back(client);
    notifyClients(client->getNickname() + " has joined the channel.", client->getNickname());
}


void IRCChannel::notifyClients(std::string message, std::string sender)
{
    for (std::vector<IRCClient *>::iterator it = users.begin(); it != users.end(); it++)
        if (sender != (*it)->getNickname())
            (*it)->sendMessages(message);
}

bool IRCChannel::isClientExists(const std::string &nickname)
{
    for (std::vector<IRCClient *>::iterator it = users.begin(); it != users.end(); it++)
        if ((*it)->getNickname() == nickname)
            return true;
    return false;
}

void IRCChannel::addOperator(IRCClient *client)
{
    users.push_back(client);
    operators.push_back(client);
}

void IRCChannel::removeMember(IRCClient *client)
{
    std::vector<IRCClient*>::iterator i = std::find(users.begin(), users.end(), client);
    if(i == users.end())
        return;
    users.erase(i);
    i = std::find(members.begin(), members.end(), client);
    if(i != members.end()){
        members.erase(i);
    }
    i = std::find(operators.begin(), operators.end(), client);
    if(i != operators.end()) {
        operators.erase(i);
    }
}

IRCClient *IRCChannel::getClient(std::string nickname) {
    for (std::vector<IRCClient *>::iterator it = users.begin(); it != users.end(); it++) {
        if ((*it)->getNickname() == nickname)
            return *it;
    }
    return NULL;
}

IRCClient *IRCChannel::getOperator(std::string nickname) {
    for (std::vector<IRCClient *>::iterator it = operators.begin(); it != operators.end(); it++) {
        if ((*it)->getNickname() == nickname)
            return *it;
    }
    return NULL;
}

void IRCChannel::setCreationTime(void)
{
    std::ostringstream oss;
    oss << time(NULL);
    creationTime = oss.str();
}

bool IRCChannel::isInviteOnly(void) const
{
    return (_inviteOnly);
}

void IRCChannel::inviteOnly(void)
{
    _inviteOnly = true;
}

void IRCChannel::unsetInviteOnly(void)
{
    _inviteOnly = false;
}

bool IRCChannel::hasKey(void) const
{
    return (_hasKey);
}

void IRCChannel::unsetKey(void)
{
    _hasKey = false;
}

bool IRCChannel::hasTopicProtection(void) const
{
    return (_hasTopicProtection);
}

void IRCChannel::setTopicProtection(void)
{
    _hasTopicProtection = true;
}

void IRCChannel::unsetTopicProtection(void)
{
    _hasTopicProtection = false;
}

bool IRCChannel::hasUserLimit(void) const
{
    return (userLimit != -1);
}

std::string IRCChannel::getCreationTime(void) const
{
    return (creationTime);
}

std::string IRCChannel::intToString(int i)
{
    std::ostringstream oss;
    
    oss << i;
    return oss.str();   
}

std::string IRCChannel::getModes(bool isOp)
{
    std::string modes;

    modes += '+';
    if (hasKey() && isOp)
        modes += 'k';
    if (hasUserLimit())
        modes += 'l';
    if (isInviteOnly())
        modes += 'i';
    if (hasTopicProtection())
        modes += 't';
    if (hasKey() && isOp)
    {
        if (modes[modes.size() - 1] != ' ')
            modes += " ";
        modes += getKey();
    }
    if (hasUserLimit())
    {
        if (modes[modes.size() - 1] != ' ')
            modes += " ";
        modes.append(intToString(getLimit()));
    }
    return (modes);
}

IRCClient *IRCChannel::getMember(std::string nickname)
{
    for (std::vector<IRCClient *>::iterator it = members.begin(); it != members.end(); it++) {
        if ((*it)->getNickname() == nickname)
            return *it;
    }
    return NULL;
}

bool IRCChannel::isMember(std::string nick)
{
    return (getMember(nick) != NULL);
}

bool IRCChannel::isOp(std::string nick)
{
    return (getOperator(nick) != NULL);
}

void IRCChannel::promoteToOp(IRCClient *client)
{
    operators.push_back(client);
    if (std::find(members.begin(), members.end(), client) != members.end())
        members.erase(std::find(members.begin(), members.end(), client));
}

void IRCChannel::demoteFromOp(IRCClient *client)
{
    members.push_back(client);
    if (std::find(operators.begin(), operators.end(), client) != operators.end())
        operators.erase(std::find(operators.begin(), operators.end(), client));
}

void IRCChannel::notifyMembers(std::string message)
{
    for (std::vector<IRCClient *>::iterator it = operators.begin(); it != operators.end(); it++)
            (*it)->sendMessages(message);
    for (std::vector<IRCClient *>::iterator it = members.begin(); it != members.end(); it++)
            (*it)->sendMessages(message);
}
