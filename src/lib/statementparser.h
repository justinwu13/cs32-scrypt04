// statementparser.h

#pragma once
#include <vector>
#include <string>
#include <map>
#include "token.h"
#include "value.h"
#include "infixparser.h"

struct StatementNode {
    Token statementToken; // empty string if statement is expression
    InfixParser::Node infixExpression; // used for everything except for Else
    std::vector<StatementNode> children;
    int indentationLevel = 0;
};

class StatementParser {
    public:
        StatementParser();
        void readStatements(std::vector<Token>& lexed);
        void fillProcedure();
        void fillSubProcedure(std::vector<StatementNode>& currBlock, unsigned int& index, int indentation); // indentation should prob be unsigned but idk for now
        void fillElif(std::vector<StatementNode>& currBlock, unsigned int& index, int indentation);
        void printProcedure();
        void printProcedureHelper(StatementNode& statement);
        void runProcedure();
        void runProcedure(std::vector<StatementNode>& currBlock);
        void runProcedureHelper(std::vector<StatementNode>& currBlock, StatementNode& currStatement, unsigned int& index);

    private: 
        int braceCounter;
        InfixParser par;
        std::vector<StatementNode> statements;
        std::vector<StatementNode> procedure;

    
};