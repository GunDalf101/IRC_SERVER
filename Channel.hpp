#include "Client.hpp"

class IRCClient;

class IRCChannel {
    private:
        std::string name;
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