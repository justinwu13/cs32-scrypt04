// statementnode.h

#pragma once
#include <vector>
#include "token.h"
#include "node.h"

struct StatementNode {
    Token statementToken; // empty string if statement is expression
    Node infixExpression; // used for everything except for Else
    std::vector<StatementNode> children;
    int indentationLevel = 0;
    std::vector<std::string> params;

    StatementNode() {};
    StatementNode(Token t) {
        statementToken = t;
    }
};