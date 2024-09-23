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

void IRCChannel::addUser(IRCClient *client)
{
    members.push_back(client);
    users.push_back(client);
    notifyClients(client->getNickname() + " has joined the channel.", client->getNickname());
}

void IRCChannel::removeUser(IRCClient *client)
{
    members.erase(std::remove(members.begin(), members.end(), client), members.end());
    users.erase(std::remove(users.begin(), users.end(), client), users.end());
    notifyClients(client->getNickname() + " has left the channel.", client->getNickname());
}

void IRCChannel::notifyClients(std::string message, std::string sender)
{
    for (std::vector<IRCClient *>::iterator it = members.begin(); it != members.end(); it++)
        if (sender != (*it)->getNickname())
            (*it)->sendMessages(message);
}

bool IRCChannel::isClientExists(const std::string &nickname)
{
    for (std::vector<IRCClient *>::iterator it = getClients().begin(); it != getClients().end(); it++)
        if ((*it)->getNickname() == nickname)
            return true;
    for (std::vector<IRCClient *>::iterator it = getOperators().begin(); it != getOperators().end(); it++)
        if ((*it)->getNickname() == nickname)
            return true;
    return false;
}

void IRCChannel::addOperator(IRCClient *client)
{
    members.push_back(client);
    operators.push_back(client);
}

void IRCChannel::removeOperator(IRCClient *client)
{
    operators.erase(std::remove(operators.begin(), operators.end(), client), operators.end());
    members.erase(std::remove(members.begin(), members.end(), client), members.end());
}

IRCClient *IRCChannel::getClient(std::string nickname) {
    //print all members
    // for (std::vector<IRCClient *>::iterator it = members.begin(); it != members.end(); it++) {
    //     std::cout << (*it)->getNickname() << std::endl;
    // }
    for (std::vector<IRCClient *>::iterator it = members.begin(); it != members.end(); it++) {
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
        modes += getKey() + " ";
    if (hasUserLimit())
        modes.append(' ' + intToString(getLimit()));
    return (modes);
}

bool IRCChannel::isMember(std::string nick)
{
    return (getClient(nick) != NULL);
}

bool IRCChannel::isOp(std::string nick)
{
    return (getOperator(nick) != NULL);
}
