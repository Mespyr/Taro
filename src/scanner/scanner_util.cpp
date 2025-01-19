#include <algorithm>

#include "scanner.hpp"

bool Scanner::is_number(std::string str) {
    if (!str.empty() && str.front() == '-') str = str.substr(1, str.size() - 1);
    return !str.empty() && std::find_if(str.begin(), str.end(), [](char c) {
                               return !std::isdigit(c);
                           }) == str.end();
}

Token::Type Scanner::get_token_type(const std::string& token) {
    if (is_number(token)) return Token::NUMBER;
    if (Token::symbols_map.count(token)) return Token::symbols_map.at(token);
    return Token::WORD;
}
