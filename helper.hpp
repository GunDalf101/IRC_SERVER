#pragma once
#include <string>
#include <vector>


// std::vector<std::string> split(const std::string &input, const std::string &delimiter) {

//     std::vector<std::string> result;

//     std::string::size_type start = 0;
//     std::string::size_type end = input.find(delimiter);

//     while (end != std::string::npos) {
//         result.push_back(input.substr(start, end - start));
//         start = end + delimiter.size();
//         end = input.find(delimiter, start);
//     }
//     result.push_back(input.substr(start));
//     return result;
// }

static std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        if(!token.empty())
            tokens.push_back(token);
    }
    return tokens;
}
