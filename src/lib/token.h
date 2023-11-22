// token.h
#pragma once
#include <string>

enum TokenType {
    Undefined = -1,
    Parenthesis = 0,
    Operator,
    Number,
    ArrayIndex,
    Identifier,
    Assignment,
    LogicOperator,
    Boolean,
    Statement,
    CurlyBrac,
    SquareBrac,
    Comma,
    Semicolon,
    Funcdef,
    Funccall,
    Invalidcall,
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