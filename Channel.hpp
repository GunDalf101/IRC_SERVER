#include "Client.hpp"

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
        // int userLimit;
        std::vector<IRCClient *> members;
        std::vector<IRCClient *> operators;
    public:
        IRCChannel(std::string name);
        ~IRCChannel();

        void addClient(IRCClient *client);
        void addOperator(IRCClient *client);
        void removeOperator(IRCClient *client);
        void removeClient(IRCClient *client);
        void notifyClients(std::string message);
        std::string getName() { return name; }
        std::vector<IRCClient *> getClients() { return members; }
        std::vector<IRCClient *> getOperators() { return operators; }
        std::string getKey() { return key; }
        void setKey(std::string key) { this->key = key; }
        void setTopic(std::string topic) { this->topic = topic; }
        std::string getTopic() { return topic; }
};