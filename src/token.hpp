#pragma once

#include <string>

#include "location.hpp"

class Token {
  public:
    enum Type {
        WORD,
        KEYWORD,
        BUILTIN,
        NUMBER,
        STRING,

        L_PAREN,
        R_PAREN,
        L_BRACE,
        R_BRACE,
        L_ANGLE,
        R_ANGLE,
        COLON,
        PERIOD,
        AT_SIGN,
        POINTER,
    };

    Token(std::string value, Type type, Location loc)
        : loc(loc), value(value), type(type) {}

    Location    loc;
    std::string value;
    Type        type;
};
