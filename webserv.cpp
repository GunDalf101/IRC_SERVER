#include "Server.hpp"

int main(int argc, char *argv[])
{
    if (argc == 3) {
        int i =  0;
        while (argv[1][i])
        {
            if (!isalnum(argv[1][i]))
            {
                std::cerr << "invalid port" << std::endl;
                return (-1);
            }
            i++;
        }
        int port = atoi(argv[1]);
        std::string pass = argv[2];
        IRCServer server(port, pass);
        server.run();
    }
    std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
    return (0);
}
