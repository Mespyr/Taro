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
        K_CALLFN,
        K_DELETE,
        K_PUSHFN,
        K_EQ,
        K_GRE,
        K_LES,
        K_GRE_EQ,
        K_LES_EQ,
        K_NOT_EQ,
        K_AND,
        K_OR,
        K_EXTERN,
        K_BIND,

        //
        WORD,
        NUMBER,
        FLOATING_POINT,
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
        EQUALS,
        QUESTION_MARK
    };

    Token(std::string value, Type type, Location loc)
        : loc(loc), value(value), type(type) {}

    Location    loc;
    std::string value;
    Type        type;

    static inline const std::unordered_map<char, Type> single_chars = {
        {'(', L_PAREN}, {')', R_PAREN}, {'[', L_BRACE}, {']', R_BRACE},
        {'<', L_ANGLE}, {'>', R_ANGLE}, {':', COLON},   {'.', PERIOD},
        {'@', AT_SIGN}, {'^', POINTER}, {'=', EQUALS},  {'?', QUESTION_MARK}};

    static inline const std::unordered_map<std::string, Type> keywords = {
        {"import", K_IMPORT}, {"expr", K_EXPR},     {"type", K_TYPE},
        {"fn", K_FN},         {"this", K_THIS},     {"cast", K_CAST},
        {"case", K_CASE},     {"else", K_ELSE},     {"end", K_END},
        {"new", K_NEW},       {"callfn", K_CALLFN}, {"delete", K_DELETE},
        {"pushfn", K_PUSHFN}, {"eq", K_EQ},         {"gre", K_GRE},
        {"les", K_LES},       {"gre-eq", K_GRE_EQ}, {"les-eq", K_LES_EQ},
        {"not-eq", K_NOT_EQ}, {"and", K_AND},       {"or", K_OR},
        {"extern", K_EXTERN}, {"bind", K_BIND}};
};
