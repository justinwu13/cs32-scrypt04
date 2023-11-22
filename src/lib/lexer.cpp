// lexer.cpp
// implements Lexer class
#include <iostream>
#include <fstream>
#include <iomanip>

#include "lexer.h"
#include "token.h"


TokenType Lexer::determineType(char c) { //determines the type of token for the specified character
    if (std::isalpha(c) || c == '_') { // could be either Identifier or Boolean if the string name is "true" or "false"
        return Identifier;
    }
    switch (c) {
        case '=': // could be either Assignment or "==" logical operator
            return Assignment;
        case '<':
        case '>':
        case '!':
        case '&':
        case '^':
        case '|':
            return LogicOperator;
        case '(':
        case ')':
            return Parenthesis;
        case '{':
        case '}':
            return CurlyBrac;
        case '[':
        case ']':
            return SquareBrac;
        case ',':
            return Comma;
        case ';':
            return Semicolon;
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
            return Operator;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '0':
            return Number;
    }  
    return Undefined;
}

void Lexer::foundError(int lineNumber, unsigned int columnNumber) { //throws errors when a syntax error is found
    std::cout << "Syntax error on line " << lineNumber << " column " << columnNumber << "." << std::endl;
    throw 1;
}

Lexer::Lexer(std::istream& input) {
    std::string line;
    int lineNumber = 1;
    
    while (!input.eof()) {
        std::getline(input, line);
        unsigned int columnNumber = 1;
        for (columnNumber = 1; columnNumber <= line.size(); columnNumber++) {
            char tokenText = line.at(columnNumber - 1);

            if(tokenText == ' ' || tokenText == '\t') {
                continue;
            }

            TokenType tokenType = determineType(tokenText);
            if (tokenType == Undefined) { // throw error
                foundError(lineNumber, columnNumber);
            } 

            else if (tokenType == Number) { // iterate through line until the number stops
                typeNumber(lineNumber, columnNumber, line, tokenType, tokenText);
            }

            else if (tokenType == Identifier) { // add characters until reaching end of the identifier name
                typeIdentifier(lineNumber,  columnNumber, line, tokenType,  tokenText);
            }

            else if (tokenType == LogicOperator) { // determine operator type and whether it should be 1/2 characters
                typeLogicOperator(lineNumber, columnNumber, line, tokenText);
            }

            else if (tokenType == Assignment) { // Check whether the equals sign is assignment or equality operator
                typeAssignment(lineNumber, columnNumber, line, tokenText);
            }

            else if (tokenType == Parenthesis) { // easy case #1
                typeParenthesis(lineNumber,  columnNumber,  tokenText);
            }

            else if (tokenType == CurlyBrac) { // easy case #2
                typeCurlyBrac(lineNumber, columnNumber, tokenText);
            }

            else if (tokenType == SquareBrac) { // easy case #3
                typeSquareBrac(lineNumber, columnNumber, tokenText);
            }

            else if (tokenType == Operator) { // easy case #4
                typeOperator(lineNumber, columnNumber, tokenText);
            }
            else if (tokenType == Comma) { // easy case #5
                typeComma(lineNumber, columnNumber, tokenText);
            }
            else if (tokenType == Semicolon) { // easy case #6
                typeSemicolon(lineNumber, columnNumber, tokenText);
            }
        } 

        if(input.eof()) {      // // gives every nonempty line its own END token
            tokens.push_back(Token(lineNumber, line.size() + 1, "END", End));
        }
        lineNumber++;
    }
}

void Lexer::typeNumber(int lineNumber, unsigned int& columnNumber, std::string line, TokenType tokenType, char tokenText) {
    std::string numberText = std::string(1, tokenText);
    unsigned int originalIndex = columnNumber;
    columnNumber++;
    bool foundPeriod = false;
                
    while(columnNumber <= line.size()) {
        tokenText = line.at(columnNumber - 1);
        tokenType = determineType(tokenText);

        if(tokenText == '.') {
            if(foundPeriod) { // throw error if a number contains more than 1 period
                foundError(lineNumber, columnNumber);
            }
            else {
                foundPeriod = true;
            }
        }
        else if(tokenType != Number) { // stops iterating if no more digits or periods are found
            break;
        }

        numberText += tokenText; //add each character to the resulting number string
        columnNumber++;
    }
    columnNumber--;

    if(numberText.at(numberText.size() - 1) == '.') { // throws an error if a period is found at the end of a number
        foundError(lineNumber, columnNumber + 1);
    }

    tokens.push_back(Token(lineNumber, originalIndex, numberText, Number));
}

void Lexer::typeIdentifier(int lineNumber, unsigned int& columnNumber, std::string line, TokenType tokenType, char tokenText) {
    std::string varName = std::string(1, tokenText);
    unsigned int originalIndex = columnNumber;
    columnNumber++;
    while (columnNumber <= line.size()) {
        tokenText = line.at(columnNumber - 1);
        tokenType = determineType(tokenText);  
        if (tokenType != Number && tokenType != Identifier) {
            break;
        }
        else {
            varName += tokenText;
        }
        columnNumber++;
    }
    columnNumber--;
    if (varName == "true" || varName == "false") { 
        tokens.push_back(Token(lineNumber, originalIndex, varName, Boolean));
    }
    else if (varName == "print" || varName == "if" || varName == "while" || varName == "else" || varName == "return") {
        tokens.push_back(Token(lineNumber, originalIndex, varName, Statement));
    }
    else if (varName == "def") {
        tokens.push_back(Token(lineNumber, originalIndex, varName, Funcdef));
    }
    else if (varName == "null") {
        tokens.push_back(Token(lineNumber, originalIndex, varName, Nullval));
    }
    else {
        tokens.push_back(Token(lineNumber, originalIndex, varName, Identifier));
    }
}

void Lexer::typeParenthesis(int lineNumber, unsigned int& columnNumber, char tokenText) {
    Token par1 = Token(lineNumber, columnNumber, std::string(1, tokenText), Parenthesis);
    tokens.push_back(par1);
}

void Lexer::typeCurlyBrac(int lineNumber, unsigned int& columnNumber, char tokenText) {
    Token curly = Token(lineNumber, columnNumber, std::string(1, tokenText), CurlyBrac);
    tokens.push_back(curly);
}

void Lexer::typeSquareBrac(int lineNumber, unsigned int& columnNumber, char tokenText) {
    Token square = Token(lineNumber, columnNumber, std::string(1, tokenText), SquareBrac);
    tokens.push_back(square);
}

void Lexer::typeOperator(int lineNumber, unsigned int& columnNumber, char tokenText) {
    Token oper1 = Token(Token(lineNumber, columnNumber, std::string(1, tokenText), Operator));
    tokens.push_back(oper1);
}

void Lexer::typeComma(int lineNumber, unsigned int& columnNumber, char tokenText) {
    Token comma = Token(lineNumber, columnNumber, std::string(1, tokenText), Comma);
    tokens.push_back(comma);
}

void Lexer::typeSemicolon(int lineNumber, unsigned int& columnNumber, char tokenText) {
    Token semicolon = Token(lineNumber, columnNumber, std::string(1, tokenText), Semicolon);
    tokens.push_back(semicolon);
}

void Lexer::typeLogicOperator(int lineNumber, unsigned int& columnNumber, std::string line, char tokenText) {
    std::string operString = std::string(1, tokenText);
    int originalIndex = columnNumber;
    if (columnNumber < line.size()) {
        char nextChar = line.at(columnNumber);
        switch (tokenText) {
            case '!':
                if (nextChar != '=') {
                    foundError(lineNumber, columnNumber);
                }
                else {
                    operString += nextChar;
                    columnNumber++;
                }
                break;
            case '<':
            case '>':
                if (nextChar == '=') {
                    operString += nextChar;
                    columnNumber++;
                }
                else {
                    // no action required
                }
                break;
            case '&':
            case '^':
            case '|':
                break; // no action required
        }
    }
    tokens.push_back(Token(lineNumber, originalIndex, operString, LogicOperator));
}

void Lexer::typeAssignment(int lineNumber, unsigned int& columnNumber, std::string line, char tokenText) {
    if (columnNumber < line.size()) {
        char nextChar = line.at(columnNumber);
        if (nextChar == '=') { // is an equality operator
            Token equals = Token(Token(lineNumber, columnNumber, "==", LogicOperator));
            tokens.push_back(equals);
            columnNumber++;
            return;
        }
    }
    Token assign = Token(Token(lineNumber, columnNumber, std::string(1, tokenText), Assignment));
    tokens.push_back(assign);
}

void Lexer::printTokens() const {
    for (unsigned int columnNumber = 0; columnNumber < tokens.size(); columnNumber++) {
        Token t = tokens.at(columnNumber);

        // possible improvement: rather than using set width, use log to figure out 
        // how much width is needed when line/column numbers get large
        std::cout << std::setw(4) << std::right << t.lineNumber << "  " << std::setw(3) << 
            t.columnNumber << "  " << std::left << t.tokenText << std::endl;
    }
}

std::vector<Token> Lexer::getVector() const {
    return tokens;
}