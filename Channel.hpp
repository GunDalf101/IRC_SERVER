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
        // ChannelMode mode;
        std::string key;
        // int userLimit;
        std::vector<IRCClient *> members;
    public:
        IRCChannel(std::string name);
        ~IRCChannel();

        void addClient(IRCClient *client);
        void removeClient(IRCClient *client);
        void notifyClients(std::string message);
        std::string getName() { return name; }
        std::vector<IRCClient *> getClients() { return members; }
};