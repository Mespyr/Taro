#include "scanner.hpp"

void Scanner::tokenize_line(const std::string& line, uint32_t line_num) {
    uint32_t start_column = find_start_col(line, 0);
    uint32_t end_column;

    while (start_column < line.length()) {
        char        c = line.at(start_column);
        std::string token_str;
        Token::Type token_type;

        if (c == '#')
            return;
        else if (c == '\'') {
            end_column = find_char_end(line, start_column);
            if (end_column >= line.length()) {
                error = std::make_unique<LocationError>(
                    Location(line_num, start_column, end_column, line,
                             current_file),
                    "unexpected EOL while tokenizing char");
                return;
            } else if (line.at(end_column) != '\'') {
                error = std::make_unique<LocationError>(
                    Location(line_num, start_column, end_column, line,
                             current_file),
                    "unexpected char found while tokenizing character");
                return;
            }
            token_type = Token::CHAR;
            end_column++;
        } else if (c == '"') {
            end_column = find_string_end(line, start_column);
            if (end_column >= line.length()) {
                error = std::make_unique<LocationError>(
                    Location(line_num, start_column, end_column, line,
                             current_file),
                    "unexpected EOL while tokenizing string");
                return;
            }
            token_type = Token::STRING;
            end_column++;
        } else if (Token::single_chars.count(c)) {
            token_type = Token::single_chars.at(c);
            end_column = start_column + 1;
            // double colon
            if (token_type == Token::COLON && end_column < line.length() &&
                line.at(end_column) == ':') {
                token_type = Token::SYM_DOUBLE_COLON;
                end_column++;
            }
        } else {
            end_column = find_end_col(line, start_column);
            std::string token_str =
                line.substr(start_column, end_column - start_column);
            token_type = get_token_type(token_str);
            // means were either dealing with a float or an error
            if (token_type == Token::NUMBER && end_column < line.length() &&
                line.at(end_column) == '.') {
                end_column++;
                if (end_column >= line.length() ||
                    std::isspace(line.at(end_column))) {
                    error = std::make_unique<LocationError>(
                        Location(line_num, start_column, end_column, line,
                                 current_file),
                        "unexpected '.' found while tokenizing number");
                    return;
                }
                uint32_t old_end_column = end_column;
                end_column = find_end_col(line, end_column);
                std::string float_decimal_section =
                    line.substr(old_end_column, end_column - old_end_column);
                if (get_token_type(float_decimal_section) != Token::NUMBER ||
                    float_decimal_section.front() == '-') {
                    error = std::make_unique<LocationError>(
                        Location(line_num, old_end_column - 1, end_column, line,
                                 current_file),
                        "unexpected '.' found while tokenizing number");
                    return;
                }
                token_type = Token::FLOATING_POINT;
            }
        }

        token_stream.insert(
            token_stream.begin() + stream_index,
            Token(line.substr(start_column, end_column - start_column),
                  token_type,
                  Location(line_num, start_column, end_column, line,
                           current_file)));

        stream_index++;
        start_column = find_start_col(line, end_column);
    }
}
