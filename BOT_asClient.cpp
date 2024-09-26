#include "BOT.hpp"

size_t BOT::sendMessage(std::string message)
{
    return send(this->clientSocket, message.c_str(), message.size(), 0);
}

void BOT::parseReply(std::string &reply)
{
    if(reply.at(0) == ':')
        reply.erase(0, 1);
    std::vector<std::string> args = toReqArgs(reply);
    std::cout << reply << std::endl;
}

bool BOT::run()
{
    if(!connectToServer())
        exit(2);
    auth();
    std::string line;
    while (true)
    {
        line = recvLine();
        if(!line.empty())
            parseReply(line);
    }
};

bool BOT::connectToServer()
{
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr(this->host.c_str());
    if(connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)))
        return false;
    return true;
};

std::string BOT::recvLine()
{
    char buffer[2] = {0};

    std::string line = "";
    
    int readBytes;
    while (true)
    {
        readBytes = read(this->clientSocket, buffer, 1);
        if(readBytes == 0)
            return line;
        buffer[readBytes] = 0;
        if(buffer[0] == '\n' || buffer[0] == '\r')
            return line;
        line.append(buffer);
    }
    return line;
}

bool BOT::auth()
{
    std::string passReq = "pass " + this->password + "\n";
    std::string nickReq = "NICK bot\n";
    std::string userReq = "USER bot 0 * :iam just a bot\n";

    this->sendMessage(passReq);
    this->sendMessage(nickReq);
    std::string reply = this->recvLine();
    if(reply.find("Password incorrect !") != std::string::npos)
    {
        std::cout << "Password incorrect !";
    }
    // if(this->recvLine().empty())
    //     std::cout << "password error." << std::endl;
    this->sendMessage(userReq);
    // char buffer[2048] = {0};
    // int bytesReceived = 1;
    // while(bytesReceived)
    // {
    //     bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    //     std::cout << buffer << std::endl;
    // }
    return true;
}












int main(int argc, char *argv[])
{
    if (argc != 1 && argc != 2) {
        int port = atoi(argv[1]);
        std::string pass = argv[2];

        BOT *bot = new BOT("127.0.0.1", port, pass);
        bot->run();

    // std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;

//     int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
//     sockaddr_in serverAddress;
//     serverAddress.sin_family = AF_INET;
//     serverAddress.sin_port = htons(port);
//     serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

//     std::string req = "pass " + pass + "\r\nNICK bot\r\nUSER bot 0 * :iam just a bot";
//     std::cout <<  connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) << std::endl;
//     send(clientSocket, req.c_str(), req.size(), 0);
//     char buffer[2048] = {0};
//     int bytesReceived = 1;
//     while(bytesReceived)
//     {
//         bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
//         std::cout << buffer << std::endl;
//     }
//     close(clientSocket);
    }
    return 0;
}