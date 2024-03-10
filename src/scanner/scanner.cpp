#include "scanner.hpp"

#include <fstream>

Scanner::Scanner(const std::string& filename) { insert(filename); }

void Scanner::insert(const std::string& filename) {
    current_file = filename;

    std::ifstream file(filename);
    if (!file.is_open()) {
        error.reset(new Error("couldn't open file '" + filename +
                              "': " + strerror(errno)));
        return;
    }

    size_t      temp = stream_index;
    std::string buf;
    uint32_t    line_num = 1;

    while (!file.eof()) {
        std::getline(file, buf);
        tokenize_line(buf, line_num);
        if (error) return;
        line_num++;
    }

    stream_index = temp;
    file.close();
}

Token Scanner::peek() { return token_stream.at(stream_index); }

Token Scanner::next() { return token_stream.at(stream_index++); }

bool Scanner::eof() { return stream_index >= token_stream.size(); }
