#include <fstream>
#include <sstream>

#include "utils.h"

std::vector<std::string> split(const std::string& str, const std::string& delimiter)
{
    std::vector<std::string> items;
    std::size_t start = 0;
    std::size_t end;

    while ((end = str.find(delimiter, start)) != std::string::npos) {
        items.emplace_back(str.substr(start, end - start));
        start = end + delimiter.length();
    }

    // Always push the last segment, even if empty
    items.emplace_back(str.substr(start));
    return items;
}

std::vector<std::string> get_clean_tokens(std::istream& input, const char comment_symbol)
{
    std::vector<std::string> tokens;
    std::string line;

    while (std::getline(input, line)) {
        // Skip comments
        if (line.empty() || line[0] == comment_symbol) continue;

        std::istringstream linestream(line);
        std::string token;
        while (linestream >> token) {
            if (!token.empty() && token[0] != comment_symbol) {
                tokens.push_back(token);
            }
            else {
                break; // comment mid-line, ignore rest of line
            }
        }
    }
    return tokens;
}