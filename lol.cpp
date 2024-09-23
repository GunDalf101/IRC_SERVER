#include <iostream>

bool isModefier(char c)
{
    return (c == '+' || c == '-');
}

void squeezeModes(std::string &modes)
{
    uint i;
    uint lastc;
    if (modes.size() == 0)
        return;
    i = lastc = 0;
    while (i < modes.size())
    {
        if (!isModefier(modes[i]))
            lastc = i;
        else if (i && isModefier(modes[i]) && i + 1 < modes.size() && !isModefier(modes[i + 1]))
        {
            if (lastc == 0)
                modes.erase(lastc, i);
            else
                modes.erase(lastc + 1, i - lastc - 1);
            i = lastc + 1;
        }
        if (i + 1 == modes.size() && isModefier(modes[i]))
            modes.erase(lastc + 1, i - lastc);
        i++;
    }
}

int main()
{
    std::string m = "++++aa-$$-++-----aaaX-----+++4$+++=---";
    squeezeModes(m);
    std::cout << m << std::endl;
}