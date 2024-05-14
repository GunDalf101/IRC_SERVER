#include "webserv.hpp"
#include <cstring>

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    int clientSock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8081);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    const char* message = "Kick me, I'm a bad server!";
    send(clientSock, message, strlen(message), 0);
    // std::fclose(clientSock);
}