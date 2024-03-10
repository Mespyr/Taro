#include "scanner.hpp"

std::optional<Token::Type> Scanner::single_char_token_type(char c) {
    switch (c) {
    case '(': return Token::L_PAREN;
    case ')': return Token::R_PAREN;
    case '[': return Token::L_BRACE;
    case ']': return Token::R_BRACE;
    case '<': return Token::L_ANGLE;
    case '>': return Token::R_ANGLE;
    case ':': return Token::COLON;
    case '.': return Token::PERIOD;
    case '@': return Token::AT_SIGN;
    case '^': return Token::POINTER;
    default: return std::nullopt;
    }
}

bool Scanner::is_single_char_token(char c) {
    return single_char_token_type(c) != std::nullopt;
}

Token::Type Scanner::get_token_type(const std::string& token) {
    return Token::WORD;
}
