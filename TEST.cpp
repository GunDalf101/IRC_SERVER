#include <iostream>
#include <vector>
#include <sstream>

std::vector<std::string> toReqArgs(std::string &params)
{
    int index = params.find(':');
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

int main(int argc, char *argv[])
{
    std::string s = "";
    std::vector<std::string> result = toReqArgs(s);

    for (const auto &s : result)
        std::cout << "{" << s << "}" << std::endl;
}