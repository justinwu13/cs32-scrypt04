// parser.cpp
// implement Parser class

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "parser.h"
#include "token.h"

// constructor for Parser, initializes values to non-junk
Parser::Parser() {
    root = Node();
    parenCounter = 0;
    variables = std::map<std::string, double>();
    expressionASTs = std::vector<Node>();
}

void Parser::clearAST() {
    root = Node();
    parenCounter = 0;
}

// helper function to throw error, prevents repeating too much code
void Parser::unexpectedTokenError(Token t) {
    std::cout << "Unexpected token at line " << t.lineNumber << " column " << t.columnNumber << ": " << t.tokenText << std::endl;
    throw 2;
}

// helper function to throw runtime error
void Parser::runTimeError(std::string output) {
    std::cout << output << std::endl;
    throw 3;
}

void Parser::fillTree(std::vector<Token>& lexed) {
    
    if(lexed.size() == 0){ // case should not happen
        return;
    }
    
    if(lexed.size() == 1) { // throws error if there is only one token (likely End token)
        Token t = lexed.at(0);
        unexpectedTokenError(t);
    }

    Token t1 = lexed.at(0);
    if(t1.tokenType == Number) {
        Token t2 = lexed.at(1);

        root = Node(t1);
        expressionASTs.push_back(root);

        // if first token is a number and the second is End, return because there is nothing more to do
        lexed.erase(lexed.begin()); 
        return;
    }

    // if the first token is not a number, it has to be "(", otherwise throw error
    if(t1.tokenText != "(") {
        unexpectedTokenError(t1);
    }

    parenCounter++;

    // check if the second token is an operator, since operator always follows "(",
    // throw error if not
    Token t2 = lexed.at(1);
    if(t2.tokenType != Operator && t2.tokenType != Assignment) {
        unexpectedTokenError(t2);
    }

    // root contains the first operator in the expression
    root = Node(t2);
    unsigned int index = 2;
    unsigned int& indexAddress = index;
    // call recursive helper on root
    fillTreeHelper(root, lexed, indexAddress);

    for(unsigned int i = 0; i <= index; i++) {
        lexed.erase(lexed.begin());
    }
    expressionASTs.push_back(root);

    return;
}

// recursively fills out the AST
// index is a reference so it can be updated from anywhere
void Parser::fillTreeHelper(Node& root, std::vector<Token>& lexed, unsigned int& index) {
    while(index < lexed.size()) {
        Token t = lexed.at(index);

        if(t.tokenText == "(") {
            leftParenthesis(root, lexed, index, t);
        }
        else if(t.tokenText == ")") {
            rightParenthesis(root, t);
            return;
        }
        else if(t.tokenType == Number) {
            if(root.data.tokenType == Assignment && (root.children.size() == 0 || root.children.at(root.children.size() - 1).data.tokenType != Identifier)) {
                unexpectedTokenError(t);
            }

            // adds a number to the current root's children
            root.children.push_back(Node(t));
        }
        else if(t.tokenType == Identifier) {
            if(root.data.tokenType == Assignment && root.children.size() > 0 && root.children.at(root.children.size() - 1).data.tokenType != Identifier) {
                unexpectedTokenError(t);
            }

            root.children.push_back(Node(t));
        }
        else if(t.tokenType == End) {
            // throw error because End shouldn't be reached, the helper function should end
            // on a ")" token
            unexpectedTokenError(t);
        }
        else {// operator token
            // operators should be covered in the "(" case, they should not be found anywhere
            // that is not directly after a "(" token
            unexpectedTokenError(t);
        }

        index++;
    }
    
}

void Parser::leftParenthesis(Node& root, std::vector<Token>& lexed, unsigned int& index, Token t){
    if(root.data.tokenType == Assignment && (root.children.size() == 0 || root.children.at(root.children.size() - 1).data.tokenType != Identifier)) {
            unexpectedTokenError(t);
    }
            
    parenCounter++;
    index++;

    // throws an error if anything other than an operator follows a "(" token
    Token t2 = lexed.at(index);
    if(t2.tokenType != Operator && t2.tokenType != Assignment) {
         unexpectedTokenError(t2);
    }

    // adds a sub-expression to the current node's children 
    // and recursively calls this function on the child sub-expression
    Node opNode = Node(t2);
    root.children.push_back(opNode);
    index++;
    fillTreeHelper(root.children.at(root.children.size() - 1), lexed, index);
}

void Parser::rightParenthesis(Node& root, Token t){
    // throws error if there are more ")" than "("
    // or if ")" is found with no elements in sub-expression
    if(parenCounter == 0 || root.children.size() == 0) {
        unexpectedTokenError(t);
    }

    if(root.data.tokenType == Assignment && root.children.size() <= 1) {
        unexpectedTokenError(t);
    }

    // sub-expression stops when ")" is found
    parenCounter--;
}

double Parser::evaluate(Node root) {
    double result = evaluateHelper(root);

    return result;
}

void Parser::equalLoop(double result){
    for(unsigned int i = 0; i < root.children.size() - 1; i++) {
        Node& n = root.children.at(i);
        if(variables.find(n.data.tokenText) == variables.end()) {
            variables.emplace(n.data.tokenText, result);
        }
        else {
            variables[n.data.tokenText] = result;
        }
    }
}

double Parser::evaluateHelper(Node root) {
    Token t = root.data;

    if(t.tokenType == Number) {
        return std::stod(t.tokenText);
    }
    if(t.tokenType == Identifier) {
        if(variables.find(t.tokenText) == variables.end()) {
            std::string errorOutput = "Runtime error: unknown identifier ";
            errorOutput += t.tokenText;
            runTimeError(errorOutput);
            return -123.4567;
        }

        return variables.at(t.tokenText);
    }

    if(root.children.size() == 0){
        return 0.0;
    }
    
    if(t.tokenText == "=") {
        double result = evaluateHelper(root.children.at(root.children.size() - 1));
        equalLoop(result);
        return result;
    }
    // if this point is reached, tokenType is Operator
    double result;
    result = evaluateHelper(root.children.at(0));

    if(t.tokenText == "+") {
        plusText(result);
    }
    else if(t.tokenText == "-") {
        minusText(result);
    }
    else if(t.tokenText == "*") {
        timesText(result);
    }
    else if(t.tokenText == "/") {
        for(unsigned int i = 1; i < root.children.size(); i++) {
            double divisor = evaluateHelper(root.children.at(i));

            if(divisor == 0.0) {
                zeroError(); // throws error if attempting to divide by zero
            }
            result /= divisor;
        }
    }
    return result;
}

double Parser::zeroError(){
    std::string output = "Runtime error: division by zero.";
    runTimeError(output);
    return -1.0; 
}

void Parser::plusText(double result){
    for(unsigned int i = 1; i < root.children.size(); i++) {
        result += evaluateHelper(root.children.at(i));
    }
}

void Parser::minusText(double result){
    for(unsigned int i = 1; i < root.children.size(); i++) {
        result -= evaluateHelper(root.children.at(i));
    }
}

void Parser::timesText(double result){
    for(unsigned int i = 1; i < root.children.size(); i++) {
        result *= evaluateHelper(root.children.at(i));
    }
}

void Parser::printAll() {
    for(Node n: expressionASTs) {
        printTree(n);
        double eval = evaluate(n);
        std::cout << eval << std::endl;
    }
}

void Parser::printTree(Node root) const {
    printTreeHelper(root);
    std::cout << std::endl;
}

void Parser::printTreeHelper(Node root) const {
    bool op = false;
    if(root.data.tokenType == Operator || root.data.tokenType == Assignment) {
        op = true;
    }

    // prints parenthesis for each operator
    if(op) {
        std::cout << "(";
    }

    if(root.data.tokenType == Number) {
        std::cout << std::stod(root.data.tokenText);
    }
    else if(root.data.tokenType == Identifier) {
        std::cout << root.data.tokenText;
    }

    // prints the operator between the children
    for(unsigned int i = 0; i < root.children.size(); i++) {
        Node n = root.children.at(i);
        if(i == 0) {
            printTreeHelper(n);
        }
        else {
            std::cout << " " << root.data.tokenText << " ";
            printTreeHelper(n);
        }
    }

    // prints parenthesis for each operator
    if(op) {
        std::cout << ")";
    }
}

