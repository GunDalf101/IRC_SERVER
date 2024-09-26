#include "BOT.hpp"

std::string BOT::toLower(std::string &s)
{
    std::string lower_s = s;
    for (size_t i = 0; i < s.length(); i++)
        lower_s[i] = tolower(s[i]);
    return lower_s;
}

void BOT::loadDatabase()
{
    std::ifstream infile("BOT_database.CSV");
    std::string line;

    std::string country;
    std::string capitale;
    std::vector<std::string> key_value;
    while(getline(infile, line))
    {
        key_value = split(line, ',');
        country = key_value[0];
        capitale = key_value[1];
        database[country] = capitale;
    }
    infile.close();
}

void BOT::execute(IRCClient *client, const std::string &params)
{
    if(!client->isAuthentificated())
        return ;
    std::vector<std::string> args = toReqArgs(params);
    if(args.size() < 1)
        return client->sendMessages(ERR_NEEDMOREPARAMS(client->getNickname(), client->getHostname(), "BOT"));
    loadDatabase();
    std::string country = toLower(args[0]);
    std::map<std::string, std::string>::iterator i = database.find(country);
    if(i == database.end())
        return client->sendMessages(PRIVMSG_FORMAT(nickname, username, server.getHostName(), client->getNickname(), "couldn't find it."));
    std::string capitale = database[country];
    std::string reply = "the capitale of " + country + " is " + capitale + ".";
    client->sendMessages(PRIVMSG_FORMAT(nickname, username, server.getHostName(), client->getNickname(), reply));
}