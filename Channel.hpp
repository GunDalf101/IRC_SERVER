#pragma once
#include "Client.hpp"

#include <ctime>
#include <sstream>

class IRCClient;

enum ChannelMode {
    INVITE_ONLY,
    TOPIC_PROTECTION,
    KEY_REQUIRED,
    OPERATOR_PRIVILEGES,
    USER_LIMIT
};

class IRCChannel {
    private:
        std::string name;
        std::string topic;
        std::string type;
        // ChannelMode mode;
        std::string key;
        int userLimit;
        std::vector<IRCClient *> members;
        std::vector<IRCClient *> users;
        std::vector<IRCClient *> operators;
    //    std::map<IRCClient *, bool> user_pairs;   to review
        std::string creationTime;
        bool _inviteOnly;
        bool _hasKey;
        bool _hasTopicProtection;
        void setCreationTime(void);
        std::string intToString(int i);
    public:
        IRCChannel(std::string name);
        ~IRCChannel();

        void addUser(IRCClient *client);
        void addOperator(IRCClient *client);
        void removeOperator(IRCClient *client);
        void removeUser(IRCClient *client);
        void notifyClients(std::string message);
        bool isClientExists(const std::string &nickname);
        std::string getName() { return name; }
        std::vector<IRCClient *> & getClients() { return users; }
        std::vector<IRCClient *> & getOperators() { return operators; }
        std::string getKey() { return key; }
        void setKey(std::string key) { this->key = key; _hasKey = true;}
        void setTopic(std::string topic) { this->topic = topic; }
        std::string getTopic() { return topic; }
        void setLimit(int limit) { userLimit = limit; }
        int getLimit() { return userLimit; }
        int getNumUsers() { return members.size(); }
        IRCClient *getClient(std::string nickname);
        IRCClient *getOperator(std::string nickname);
        std::string getCreationTime(void) const;
        bool isInviteOnly(void) const;
        void inviteOnly(void);
        void unsetInviteOnly(void);
        bool hasKey(void) const;
        void unsetKey(void);
        bool hasTopicProtection(void) const;
        void setTopicProtection(void);
        void unsetTopicProtection(void);
        bool hasUserLimit(void) const;
        std::string getModes(bool isOp);
        bool isMember(std::string nick);
        bool isOp(std::string nick);
};
