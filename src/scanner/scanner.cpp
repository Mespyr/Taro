#include "scanner.hpp"

Scanner::Scanner(const std::string& filename) { insert(filename); }

void Scanner::insert(const std::string& filename) {
    std::shared_ptr<File> file = std::make_unique<File>(filename);
    if (!file->is_open()) {
        error = std::make_unique<Error>("couldn't open file '" + filename +
                                        "': " + strerror(errno));
        return;
    }

    size_t   temp = stream_index;
    uint32_t line_num = 0;
    while (line_num < file->line_count()) {
        tokenize_line(line_num++, file);
        if (error) return;
    }
    stream_index = temp;
}

Token Scanner::peek() { return token_stream.at(stream_index); }

Token Scanner::next() { return token_stream.at(stream_index++); }

bool Scanner::eof() { return stream_index >= token_stream.size(); }
