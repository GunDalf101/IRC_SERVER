#include <iostream>
#include <vector>
#include <sstream>

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
    std::string s = "a b c :     d";
    std::vector<std::string> result = split(s, ' ');

    for (const auto &s : result)
        std::cout << "{" << s << "}" << std::endl;
}