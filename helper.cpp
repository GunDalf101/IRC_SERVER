#include "webserv.hpp"

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

std::vector<std::string> toReqArgs(const std::string &params)
{
    size_t index = params.find(':');
    std::string co = params;
    if(index != std::string::npos)
        co = params.substr(0, index);

    std::vector<std::string> substrings;
    std::stringstream ss(co);
    std::string param;
    
    while (std::getline(ss, param, ' '))
        if(!param.empty())
            substrings.push_back(param);

    if (index != std::string::npos)
        substrings.push_back(params.substr(index + 1, params.length() - index - 1));

    return substrings;
}