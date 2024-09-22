#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    if (argc != 1 && argc != 2) {
        int port = atoi(argv[1]);
        std::string pass = argv[2];
    // std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    std::cout <<  connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) << std::endl;
    // std::string s = "PASS a\n\r" + pass + "\nNICK bot\nUSER bot a a a";
    send(clientSocket, "PASS coco\r\n", strlen("PASS coco\r\n"), 0);
    send(clientSocket, "NICK bot\r\n", strlen("NICK bot\r\n"), 0);
    send(clientSocket, "USER bot a a a\r\n", strlen("USER bot a a a\r\n"), 0);
    // send(clientSocket, s.c_str(), strlen(s.c_str()), 0);
    char buffer[2048] = {0};
    int bytesReceived = 1;
    while(bytesReceived)
    {
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        std::cout << buffer << std::endl;
    }
    close(clientSocket);
    }
    return 0;
}