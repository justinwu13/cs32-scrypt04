// token.h
#pragma once
#include <string>

enum TokenType {
    Undefined = -1,
    Parenthesis = 0,
    Operator,
    Number,
    Identifier,
    Assignment,
    LogicOperator,
    Boolean,
    Statement,
    CurlyBrac,
    End
};

struct Token {
    int lineNumber;
    int columnNumber;
    std::string tokenText;
    TokenType tokenType;

    Token(int line = -1, int column = -1, std::string text = "", TokenType type = Undefined) {
        lineNumber = line;
        columnNumber = column;
        tokenText = text;
        tokenType = type;
    }
};

