// infixparser.h

#pragma once
#include <vector>
#include <string>
#include <map>
#include "token.h"
#include "value.h"

class InfixParser {
    public:
        struct Node{
            Token data;
            std::vector<Node> children;

            Node() {
                data = Token();
                std::vector<Node> children = std::vector<Node>();
            }

            Node(Token data){
                this->data = data;
                children = std::vector<Node>();
            }
        };

        Node root;

        InfixParser();
        void fillTreeInfix(std::vector<Token>& lexed);
        Node fillTreeSubexpression(std::vector<Token>& lexed, unsigned int& index);
        Node fillTreeInfixHelper(std::vector<Token>& lexed, unsigned int& index, unsigned int currPrecedence);
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
        
    private:
        int parenCounter;
        std::map<std::string, Value> variables; // updates all variables even if hitting runtime error
        std::map<std::string, Value> variablesStorage; // used to reset variables to previous state in case of runtime error
        std::vector<Node> expressionASTs;

        double zeroError();
        void checkValidity(Value& param1, Value& param2, std::string op);

};