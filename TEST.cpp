#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        if(!token.empty())
            tokens.push_back(token);
    }
    return tokens;
}

int main(int argc, char *argv[])
{
    std::map<std::string,std::string> database;
    std::cout << database.size() << std::endl;
}