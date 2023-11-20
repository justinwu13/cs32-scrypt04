// format.cpp

#include "lib/lexer.h"
#include "lib/statementparser.h"
#include "lib/token.h"
#include "lib/value.h"
#include <iostream>
#include <iomanip>
#include <vector>

int main() {
    StatementParser par;

    std::string line;
    while(std::getline(std::cin, line)) {
        std::istringstream stream(line);

        try {
            Lexer lex(stream);
            std::vector<Token> tokens = lex.getVector();
            std::cout << std::boolalpha;
            par.readStatements(tokens);
            

        }
        catch(int errorCode) {
            return errorCode;
        }
        
    }
    try {
        par.fillProcedure();
        par.printProcedure();
    }
    catch(int errorCode) {
        return errorCode;
    }

    return 0;
}

//testing Visual Studio, please ignore