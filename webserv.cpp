#include "Server.hpp"

int main(int argc, char *argv[])
{
    if (argc != 1 && argc != 2) {
        int port = atoi(argv[1]);
        std::string pass = argv[2];
        IRCServer server(port, pass);
        server.run();
    }
    std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
}