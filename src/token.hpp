#pragma once

#include <string>
#include <unordered_map>

#include "location/location.hpp"

class Token {
  public:
    enum Type {
        // keywords
        K_IMPORT,
        K_EXPR,
        K_TYPE,
        K_FN,
        K_EXTERN,

        K_IF,
        K_ELSE,
        K_WHILE,

        K_AS,
        K_CALL,

        K_EQ,
        K_GRE,
        K_LES,
        K_AND,
        K_OR,

        // literals
        WORD,
        NUMBER,
        FLOATING_POINT,
        STRING,
        CHAR,

        // symbol
        SYM_ARROW,

        // single char / delimiters
        L_PAREN,
        R_PAREN,
        L_BRACKET,
        R_BRACKET,
        L_BRACE,
        R_BRACE,
        COLON,
        ACCESS_MEMBER,
        EQUALS,
        PUSH_FUNCTION  // '\' (backslash)
    };

    Token(const std::string &value, Type type, const Location &loc)
        : value(value), type(type), loc(loc) {}

    std::string value;
    Type        type;
    Location    loc;

    static inline const std::unordered_map<char, Type> single_chars = {
        {'(', L_PAREN},       {')', R_PAREN},       {'[', L_BRACKET},
        {']', R_BRACKET},     {'{', L_BRACE},       {'}', R_BRACE},
        {':', COLON},         {'.', ACCESS_MEMBER}, {'=', EQUALS},
        {'\\', PUSH_FUNCTION}};

    static inline const std::unordered_map<std::string, Type> symbols = {
        {"->", SYM_ARROW}};

    static inline const std::unordered_map<std::string, Type> keywords = {
        {"import", K_IMPORT}, {"expr", K_EXPR},     {"type", K_TYPE},
        {"fn", K_FN},         {"extern", K_EXTERN}, {"if", K_IF},
        {"else", K_ELSE},     {"while", K_WHILE},   {"as", K_AS},
        {"call", K_CALL},     {"eq", K_EQ},         {">", K_GRE},
        {"<", K_LES},         {"and", K_AND},       {"or", K_OR}};
};
