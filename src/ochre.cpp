#include <iostream>

#include "error/error.hpp"
#include "scanner/scanner.hpp"
#include "token.hpp"

// arent large switch statements just so pretty
// and totally not an eyesore
// especially for this usecase??
const std::string t_type(Token::Type t) {
    switch (t) {
    case Token::WORD: return "WORD";
    case Token::NUMBER: return "NUMBER";
    case Token::STRING: return "STRING";
    case Token::CHAR: return "CHAR";
    case Token::L_PAREN: return "L_PAREN";
    case Token::R_PAREN: return "R_PAREN";
    case Token::L_BRACE: return "L_BRACE";
    case Token::R_BRACE: return "R_BRACE";
    case Token::L_ANGLE: return "L_ANGLE";
    case Token::R_ANGLE: return "R_ANGLE";
    case Token::COLON: return "COLON";
    case Token::PERIOD: return "PERIOD";
    case Token::AT_SIGN: return "ATSIGN";
    case Token::POINTER: return "POINTER";
    case Token::EQUALS: return "EQUALS";
    case Token::K_IMPORT: return "K_IMPORT";
    case Token::K_EXPR: return "K_EXPR";
    case Token::K_TYPE: return "K_TYPE";
    case Token::K_FN: return "K_FN";
    case Token::K_THIS: return "K_THIS";
    case Token::K_CAST: return "K_CAST";
    case Token::K_CASE: return "K_CASE";
    case Token::K_ELSE: return "K_ELSE";
    case Token::K_END: return "K_END";
    case Token::K_NEW: return "K_NEW";
    case Token::K_CALL1:
    case Token::K_CALL2:
    case Token::K_CALL3:
    case Token::K_CALL4:
    case Token::K_CALL5:
    case Token::K_CALL6: return "syscall";
    case Token::K_CALLFN: return "K_CALLFN";
    case Token::K_DELETE: return "K_DELETE";
    case Token::K_PUSHFN: return "K_PUSHFN";
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
        if (t.loc.end_column - t.loc.start_column >= 8) {
            std::cout << "\t" << t_type(t.type) << std::endl;
        } else
            std::cout << "\t\t" << t_type(t.type) << std::endl;
    }

    return 0;
}
