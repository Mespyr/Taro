#include "scanner.hpp"

void Scanner::tokenize_line(const std::string& line, uint32_t line_num) {
    uint32_t start_column = find_start_col(line, 0);
    uint32_t end_column;

    while (start_column < line.length()) {
        char c = line.at(start_column);

        if (c == '#')
            return;
        else if (c == '"') {
            end_column = find_string_end(line, start_column);
            if (end_column == line.length()) {
                error.reset(new LocationError(
                    Location(line_num, start_column, end_column, line,
                             current_file),
                    "unexpected EOL while tokenizing string"));
                return;
            }
            end_column++;
            token_stream.insert(
                token_stream.begin() + stream_index,
                Token(line.substr(start_column, end_column - start_column),
                      Token::STRING,
                      Location(line_num, start_column, end_column, line,
                               current_file)));
        } else if (is_single_char_token(c)) {
            token_stream.insert(
                token_stream.begin() + stream_index,
                Token(line.substr(start_column, 1),
                      single_char_token_type(c).value(),
                      Location(line_num, start_column, start_column + 1, line,
                               current_file)));
            end_column = start_column + 1;
        } else {
            end_column = find_end_col(line, start_column);
            std::string str =
                line.substr(start_column, end_column - start_column);
            Token::Type type = get_token_type(str);
            token_stream.insert(
                token_stream.begin() + stream_index,
                Token(str, type,
                      Location(line_num, start_column, end_column, line,
                               current_file)));
        }

        stream_index++;
        start_column = find_start_col(line, end_column);
    }
}
