// parse.cpp

#include "lib/lexer.h"
#include "lib/parser.h"
#include "lib/token.h"
#include <iostream>
#include <iomanip>
#include <vector>

int main() {
    std::vector<Token> tokenVec;
    try {
        Lexer lex(std::cin);
        tokenVec = lex.getVector();
    }
    catch(int errorCode) {
        return errorCode;
    }

    Parser par;

    try {
        while(tokenVec.size() > 1) {
            par.fillTree(tokenVec);
            par.clearAST();
        }
        par.printAll();
    }
    catch(int errorCode) {
        return errorCode;
    }
    
    
    
    return 0;
}