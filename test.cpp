#include <iostream>

bool isValidChar(char c)
{
    std::string special = "-_\\[]{}^|";
    if(isalnum(c))
        return true;
    for(int i = 0; i < special.length(); i++)
        if(special.at(i) == c)
            return true;
    return false;
}

bool isValidNickUser(const std::string &nickname)
{
    if(nickname.length() > 30)
        return false;
    if(!isalpha(nickname.at(0)))
        return false;
    for(int i = 0; i < nickname.length(); i++)
        if(!isValidChar(nickname.at(i)))
            return false;
    return true;
}

int main(int argc, char const *argv[])
{
    std::string a = argv[1];
    std::cout << isValidNickUser(a) << std::endl;
    return 0;
}
