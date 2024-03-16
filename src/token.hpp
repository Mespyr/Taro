#pragma once

#include <string>
#include <unordered_map>

#include "location.hpp"

class Token {
  public:
    enum Type {
        // keywords
        K_IMPORT,
        K_EXPR,
        K_TYPE,
        K_FN,
        K_THIS,
        K_CAST,
        K_CASE,
        K_ELSE,
        K_END,
        K_NEW,
        K_CALL1,
        K_CALL2,
        K_CALL3,
        K_CALL4,
        K_CALL5,
        K_CALL6,
        K_CALLFN,
        K_DELETE,
        K_PUSHFN,

        //
        WORD,
        NUMBER,
        STRING,
        CHAR,

        // single char / delimiters
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
        EQUALS
    };

    Token(std::string value, Type type, Location loc)
        : loc(loc), value(value), type(type) {}

    Location    loc;
    std::string value;
    Type        type;

    static inline const std::unordered_map<char, Type> single_chars = {
        {'(', L_PAREN}, {')', R_PAREN}, {'[', L_BRACE}, {']', R_BRACE},
        {'<', L_ANGLE}, {'>', R_ANGLE}, {':', COLON},   {'.', PERIOD},
        {'@', AT_SIGN}, {'^', POINTER}, {'=', EQUALS}};

    static inline const std::unordered_map<std::string, Type> keywords = {
        {"import", K_IMPORT}, {"expr", K_EXPR},     {"type", K_TYPE},
        {"fn", K_FN},         {"this", K_THIS},     {"cast", K_CAST},
        {"case", K_CASE},     {"else", K_ELSE},     {"end", K_END},
        {"new", K_NEW},       {"call1", K_CALL1},   {"call2", K_CALL2},
        {"call3", K_CALL3},   {"call4", K_CALL4},   {"call5", K_CALL5},
        {"call6", K_CALL6},   {"callfn", K_CALLFN}, {"delete", K_DELETE},
        {"pushfn", K_PUSHFN}};
};
