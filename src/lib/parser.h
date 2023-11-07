// parser.h
#pragma once
#include <vector>
#include <string>
#include <map>
#include "token.h"

class Parser {
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

        Parser();
        void fillTree(std::vector<Token>& lexed);
        void fillTreeHelper(Node& root, std::vector<Token>& lexed, unsigned int& index);
        void printTree(Node root) const;
        void printTreeHelper(Node root) const;
        void leftParenthesis(Node& root, std::vector<Token>& lexed, unsigned int& index, Token t);
        void rightParenthesis(Node& root, Token t);
        double zeroError();
        void equalLoop(double result);
        void plusText(double result);
        void minusText(double result);
        void timesText(double result);
        void printAll();
        double evaluate(Node root);
        double evaluateHelper(Node root);
        void clearAST();

    private:
        Node root;
        int parenCounter;
        std::map<std::string, double> variables;
        std::vector<Node> expressionASTs;
        void unexpectedTokenError(Token t);
        void runTimeError(std::string output);
};