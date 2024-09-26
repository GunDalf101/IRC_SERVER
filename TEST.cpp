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
    std::ifstream infile("BOT_database.CSV");
    std::string line;

    std::map<std::string,std::string> database;
    std::string country;
    std::string capitale;
    std::vector<std::string> key_value;
    while(getline(infile, line))
    {
        key_value = split(line, ',');
        country = key_value[0];
        capitale = key_value[1];
        std::cout << "{" << capitale << "}" << std::endl;
        database[country] = capitale;
        std::cout << database.size() << std::endl;
    }
}