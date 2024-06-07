#include <iostream>

#include "error/error.hpp"
#include "scanner/scanner.hpp"
#include "token.hpp"

// arent large switch statements just so pretty
// and totally not an eyesore
// especially for this usecase??
const std::string t_type(Token::Type t) {
    switch (t) {
    case Token::SYM_ARROW: return "SYM_ARROW";
    case Token::WORD: return "WORD";
    case Token::NUMBER: return "NUMBER";
    case Token::FLOATING_POINT: return "FLOATING POINT";
    case Token::STRING: return "STRING";
    case Token::CHAR: return "CHAR";
    case Token::L_PAREN: return "L_PAREN";
    case Token::R_PAREN: return "R_PAREN";
    case Token::L_BRACE: return "L_BRACE";
    case Token::R_BRACE: return "R_BRACE";
    case Token::L_BRACKET: return "L_BRACKET";
    case Token::R_BRACKET: return "R_BRACKET";
    case Token::COLON: return "COLON";
    case Token::ACCESS_MEMBER: return "ACCESS_MEMBER";
    case Token::POINTER: return "POINTER";
    case Token::PUSH_FUNCTION: return "PUSH FUNCTION";
    case Token::EQUALS: return "EQUALS";
    case Token::K_IMPORT: return "K_IMPORT";
    case Token::K_EXPR: return "K_EXPR";
    case Token::K_TYPE: return "K_TYPE";
    case Token::K_FN: return "K_FN";
    case Token::K_IF: return "K_IF";
    case Token::K_ELSE: return "K_ELSE";
    case Token::K_WHILE: return "K_WHILE";
    case Token::K_CALL: return "K_CALL";
    case Token::K_EXTERN: return "K_EXTERN";
    case Token::K_AS: return "K_AS";

    case Token::K_EQ: return "K_EQ";
    case Token::K_GRE: return "K_GRE";
    case Token::K_LES: return "K_LES";
    case Token::K_AND: return "K_AND";
    case Token::K_OR: return "K_OR";
    }
    return "";
}

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        std::cerr << Error("no file provied for compilation").to_string()
                  << std::endl;
        return 1;
    }

    Scanner scanner(argv[1]);
    if (scanner.error) {
        std::cerr << scanner.error->to_string() << std::endl;
        return 1;
    }

    while (!scanner.eof()) {
        Token t = scanner.next();
        std::cout << "(" << t.loc.line_num << ")\t" << t.value;
        if (t.loc.end_column - t.loc.start_column >= 8)
            std::cout << "\t" << t_type(t.type) << std::endl;
        else
            std::cout << "\t\t" << t_type(t.type) << std::endl;
    }
    return 0;
}
