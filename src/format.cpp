// format.cpp

#include "lib/lexer.h"
#include "lib/infixparser.h"
#include "lib/token.h"
#include "lib/value.h"
#include "lib/statementparser.h"
#include <iostream>
#include <iomanip>
#include <vector>

int main() {
    StatementParser par;
    std::vector<Token> tokens;
    Token last;

    std::string line;
    while(std::getline(std::cin, line)) {
        std::istringstream stream(line);

        try {
            Lexer lex(stream);
            std::vector<Token> currLine = lex.getVector();
            for(Token t: currLine) {
                if(t.tokenType == End) {
                    last = t;
                }
                else {
                    tokens.push_back(t);
                }
            }
        }
        catch(int errorCode) {
            return errorCode;
        }
        
    }

    tokens.push_back(last);

    try {
        std::cout << std::boolalpha;
        par.readStatements(tokens);
        par.fillProcedure();
        par.printProcedure();
    }
    catch(int errorCode) {
        return errorCode;
    }

    return 0;
}

