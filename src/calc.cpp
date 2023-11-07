// calc.cpp

#include "lib/lexer.h"
#include "lib/infixparser.h"
#include "lib/token.h"
#include "lib/value.h"
#include <iostream>
#include <iomanip>
#include <vector>

int main() {
    InfixParser par;
    
    std::string line;
    while(std::getline(std::cin, line)) {
        std::istringstream stream(line);

        try {
            Lexer lex(stream);
            std::vector<Token> tokens = lex.getVector();
            par.fillTreeInfix(tokens);
            par.printTree(par.root);
            std::cout << std::endl;
            Value eval = par.evaluate(par.root);
            std::cout << std::boolalpha;
            std::cout << eval << std::endl;
            par.updateVariables();
        }
        catch(int errorCode) {
            par.resetVariables();
        }
        
        par.clearAST();
    }

    return 0;
}