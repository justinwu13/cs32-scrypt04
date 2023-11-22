// statementparser.h

#pragma once
#include <vector>
#include <string>
#include <map>
#include "token.h"
#include "infixparser.h"
#include "statementnode.h"

class StatementParser {
    public:
        StatementParser();
        void readStatements(std::vector<Token>& lexed);
        Node readExpression(std::vector<Token>& lexed, unsigned int& index, std::string delimiter);
        void fillProcedure();
        void fillSubProcedure(std::vector<StatementNode>& currBlock, unsigned int& index, int indentation); // indentation should prob be unsigned but idk for now
        void fillElif(std::vector<StatementNode>& currBlock, unsigned int& index, int indentation);
        void fillFunc(StatementNode& s, unsigned int& index, int indentation);
        void printProcedure();
        void printProcedureHelper(StatementNode& statement);
        std::vector<StatementNode> getProcedure();

    private: 
        int braceCounter;
        InfixParser par;
        std::vector<StatementNode> statements;
        std::vector<StatementNode> procedure;

        std::map<std::string, Function> functions;
    
};