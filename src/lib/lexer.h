// lexer.h

#pragma once
#include "token.h"
#include <stack>
#include <vector>
#include <string>


class Lexer {
    private:
        std::vector<Token> tokens;

    public:
        TokenType determineType(char c);
        std::string fileName;
        void foundError(int lineNumber, unsigned int columnNumber);
        Lexer(std::istream& input);
        void typeNumber(int lineNumber, unsigned int& columnNumber, std::string line, TokenType tokenType, char tokenText);
        void typeIdentifier(int lineNumber, unsigned int& columnNumber, std::string line, TokenType tokenType, char tokenText);
        void typeParenthesis(int lineNumber, unsigned int& columnNumber, char tokenText);
        void typeCurlyBrac(int lineNumber, unsigned int& columnNumber, char tokenText);
        void typeSquareBrac(int lineNumber, unsigned int& columnNumber, char tokenText);
        void typeOperator(int lineNumber, unsigned int& columnNumber, char tokenText);
        void typeComma(int lineNumber, unsigned int& columnNumber, char tokenText);
        void typeSemicolon(int lineNumber, unsigned int& columnNumber, char tokenText);
        void typeLogicOperator(int lineNumber, unsigned int& columnNumber, std::string line, char tokenText);
        void typeAssignment(int lineNumber, unsigned int& columnNumber, std::string line, char tokenText);
        void printTokens() const;
        std::vector<Token> getVector() const;
};