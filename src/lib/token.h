// token.h
#pragma once
#include <string>
#include "value.h"

enum TokenType {
    Undefined = -1,
    Parenthesis = 0,
    Operator,
    Number,
    Array,
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

struct Array: public Token {
    std::vector<Value> array;

    Array(int line = -1, int column = -1, std::string text = "", TokenType type = Undefined) {
        lineNumber = line;
        columnNumber = column;
        tokenText = text;
        tokenType = type;
    }
};