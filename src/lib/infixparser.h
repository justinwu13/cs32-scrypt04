// infixparser.h

#pragma once
#include <vector>
#include <string>
#include <map>
#include "token.h"
#include "value.h"
#include "node.h"
#include "function.h"

class InfixParser {
    public:
        Node root;

        InfixParser();
        void fillTreeInfix(std::vector<Token>& lexed);
        Node fillTreeSubexpression(std::vector<Token>& lexed, unsigned int& index);
        Node fillTreeInfixHelper(std::vector<Token>& lexed, unsigned int& index, unsigned int currPrecedence);
        void fillFunctionCall(Node& parent, std::vector<Token>& lexed, unsigned int& index);
        Node buildArray(std::vector<Token>& lexed, unsigned int& index);
        void printTree(Node root) const;
        void printTreeHelper(Node root) const;
        void printAll();
        void clearAST();
        void updateVariables();
        void resetVariables();
        void diffCases(Value& result, Node& root, Token& t);
        void equalText(Value& result, Node& root);
        void plusText(Value& result, Node& root, Token& t);
        void minusText(Value& result, Node& root, Token& t);
        void timesText(Value& result, Node& root, Token& t);
        void divideText(Value& result, Node& root, Token& t);
        void modText(Value& result, Node& root, Token& t);
        void lessText(Value& result, Node& root, Token& t);
        void lessequalText(Value& result, Node& root, Token& t);
        void greaterText(Value& result, Node& root, Token& t);
        void greaterequalText(Value& result, Node& root, Token& t);
        void doubleEqual(Value& result, Node& root);
        void notEqual(Value& result, Node& root);
        void andText(Value& result, Node& root, Token& t);
        void orText(Value& result, Node& root, Token& t);
        void caretText(Value& result, Node& root, Token& t);
        
        Value evaluate(Node root);
        Value evaluateHelper(Node root);

        void unexpectedTokenError(Token t);
        void runTimeError(std::string output);

        Value runProcedure(std::vector<StatementNode>& currBlock, bool isFunc);
        Value runProcedureHelper(std::vector<StatementNode>& currBlock, StatementNode& s, unsigned int& index, bool isFunc);
        
        void emplaceVariable(std::string key, Value mapped);
        std::map<std::string, Function> functions;

    private:
        int parenCounter;
        // this organization is based on when runtime errors did not cause the program to exit
        std::map<std::string, Value> variables; // updates all variables even if hitting runtime error (obsolete)
        std::map<std::string, Value> variablesStorage; // used to reset variables to previous state in case of runtime error (obsolete)
        std::vector<Node> expressionASTs;

        double zeroError();
        void checkValidity(Value& param1, Value& param2, std::string op);

};