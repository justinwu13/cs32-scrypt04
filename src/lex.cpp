#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cstring>

#include "lib/lexer.h"

int main() { 
    try {
        Lexer lexer(std::cin);

        lexer.printTokens();
    }
    catch(int errorCode) { 
        return errorCode;
    }

    return 0;
}