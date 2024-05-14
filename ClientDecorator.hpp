#pragma once
#include "Client.hpp"

class IRCClient;

class AuthenticatedClient: public IRCClient {
    private:
        IRCClient *wrappedClient;
    public:
        AuthenticatedClient(IRCClient *client);
        ~AuthenticatedClient();

        bool isAuthentificated() { return true; }
};

class Operator: public AuthenticatedClient {
    public:
        Operator(IRCClient *client);
        ~Operator();

        bool isOperator() { return true; }
};