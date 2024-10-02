#include "Server.hpp"

int main(int argc, char *argv[])
{
    std::cout << getpid() << std::endl;
    if (argc != 1 && argc != 2) {
        try {
            int port = atoi(argv[1]);
            std::string pass = argv[2];
            IRCServer server(port, pass);
            server.run();
        } catch (...) {
            std::cout << "error zomal" << std::endl;
        }
    }
    std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
}