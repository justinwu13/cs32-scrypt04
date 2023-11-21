// infixparser.cpp
// implement InfixParser class

#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <stack>
#include "infixparser.h"
#include "token.h"

// constructor, initializes values to non-junk
InfixParser::InfixParser() {
    root = Node();
    parenCounter = 0;
}

void InfixParser::clearAST() {
    root = Node();
    parenCounter = 0;
}

void InfixParser::updateVariables() {
    variablesStorage = variables;
}

void InfixParser::resetVariables() {
    variables = variablesStorage;
}

// helper function to throw error for unexpected token, prevents repeating too much code
void InfixParser::unexpectedTokenError(Token t) {
    std::cout << "Unexpected token at line " << t.lineNumber << " column " << t.columnNumber << ": " << t.tokenText << std::endl;
    throw 2;
}

// helper function to throw runtime error
void InfixParser::runTimeError(std::string output) {
    std::cout << output << std::endl;
    throw 3;
}

Value InfixParser::evaluate(Node root) {
    return evaluateHelper(root);
}

void InfixParser::checkValidity(Value& param1, Value& param2, std::string op) { // determines if the data types of the values are valid for the given operation
    if (op == "+" || op == "-" || op == "*" || op == "/" || op == "%" || op.at(0) == '>' || op.at(0) == '<') {
        if (!(param1.type == Value::DOUBLE && param2.type == Value::DOUBLE)) {
            runTimeError("Runtime error: invalid operand type.");
        }
    }
    else if (op == "&" || op == "^" || op == "|") {
        if (!(param1.type == Value::BOOL && param2.type == Value::BOOL)) {
            runTimeError("Runtime error: invalid operand type.");
        }
    }
    else if (op == "==" || op == "!=") { //deprecated case
        if (param1.type != param2.type) {
            runTimeError("Runtime error: invalid operand type.");
        }
    }
}

//helper functions for evaluateHelper begin here:
double InfixParser::zeroError(){
    runTimeError("Runtime error: division by zero.");
    return -1.0;
}

void InfixParser::diffCases(Value& result, Node& root, Token& t){
    if(t.tokenText == "+") {
        plusText(result,root,t);
    }
    else if(t.tokenText == "-") {
        minusText(result,root,t);
    }
    else if(t.tokenText == "*") {
        timesText(result,root,t);
    }
    else if(t.tokenText == "/") {
        divideText(result,root,t);
    }
    else if(t.tokenText == "%") {
        modText(result,root,t);
    }
    else if (t.tokenText == "<") {
        lessText(result,root,t);
    }
    else if (t.tokenText == "<=") {
        lessequalText(result,root,t);
    }
    else if (t.tokenText == ">") {
        greaterText(result,root,t);
    }
    else if (t.tokenText == ">=") {
        greaterequalText(result,root,t);
    }
    else if (t.tokenText == "==") {
        doubleEqual(result,root);
    }
    else if (t.tokenText == "!=") {
        notEqual(result,root);
    }
    else if (t.tokenText == "&") {
        andText(result,root,t);
    }
    else if (t.tokenText == "|") {
        orText(result,root,t);
    }
    else if (t.tokenText == "^") {
        caretText(result,root,t);
    }
}

void InfixParser::equalText(Value& result, Node& root){
    for(unsigned int i = 0; i < root.children.size() - 1; i++) {// checks all children except rightmost
        Node& n = root.children.at(i);
        if (n.data.tokenType == Boolean || n.data.tokenType == Number) {
            for (unsigned int j = 0; j < n.children.size(); j++) {
                if (!n.children.empty() && n.children.at(j).data.tokenType == ArrayIndex) {
                    std::string output = "Runtime error: not an array.";
                    runTimeError(output);
                }
            }
            std::string output = "Runtime error: invalid assignee.";
            runTimeError(output);
        }
        if (n.data.tokenText == "[") {
            for (unsigned int j = 0; j < n.children.size(); j++) {
                if (!n.children.empty() && n.children.at(j).data.tokenType == ArrayIndex) {
                    return;
                }
            }
        }
        if(n.data.tokenType != Identifier) {// throw error if not variable
            std::string output = "Runtime error: invalid assignee.";
            runTimeError(output);
        }
        if(variables.find(n.data.tokenText) == variables.end()) {// create variable data
            if (!n.children.empty() && n.children.at(0).data.tokenType == ArrayIndex) {
                std::string output = "Runtime error: not an array.";
                runTimeError(output);
            }
            variables.emplace(n.data.tokenText, result);
        }
        else {// update variable if variable has existing value
            unsigned int j = 0;
            if (!n.children.empty() && n.children.at(j).data.tokenType == ArrayIndex) {
                Value* v = &variables[n.data.tokenText];
                if (v->type != Value::ARRAY) {
                    std::string output = "Runtime error: not an array.";
                    runTimeError(output);
                }
                Value arrIndex = evaluateHelper(n.children.at(j).children.at(0));
                while (j < n.children.size() && n.children.at(j).data.tokenType == ArrayIndex) { // multidimensional array handling
                    if (arrIndex.type != Value::DOUBLE) {
                        std::string output = "Runtime error: index is not a number.";
                        runTimeError(output);
                    }
                    double filler; // used solely as parameter for modf function
                    if (modf(arrIndex.double_value, &filler) != 0) {
                        std::string output = "Runtime error: index is not an integer.";
                        runTimeError(output);
                    }
                    if (arrIndex.double_value >= v->arr_value->size() || arrIndex.double_value < 0) {
                        std::string output = "Runtime error: index out of bounds.";
                        runTimeError(output);
                    }
                    v = &v->arr_value->at(arrIndex.double_value); // update v to point to the value needed to change
                    j++;
                }
                *v = result;
            }
            else {
                variables[n.data.tokenText] = result;
            }
        }
    }
}

void InfixParser::plusText(Value& result, Node& root, Token& t){
    for(unsigned int i = 1; i < root.children.size(); i++) {
        Value intermediate = evaluateHelper(root.children.at(i));
        checkValidity(result, intermediate, t.tokenText);
        result.double_value += evaluateHelper(root.children.at(i)).double_value;
    }
}

void InfixParser::minusText(Value& result, Node& root, Token& t){
    for(unsigned int i = 1; i < root.children.size(); i++) {
        Value intermediate = evaluateHelper(root.children.at(i));
        checkValidity(result, intermediate, t.tokenText);
        result.double_value -= evaluateHelper(root.children.at(i)).double_value;
    }
}

void InfixParser::timesText(Value& result, Node& root, Token& t){
    for(unsigned int i = 1; i < root.children.size(); i++) {
        Value intermediate = evaluateHelper(root.children.at(i));
        checkValidity(result, intermediate, t.tokenText);
        result.double_value *= evaluateHelper(root.children.at(i)).double_value;
    }
}
void InfixParser::divideText(Value &result, Node &root, Token &t){
    for(unsigned int i = 1; i < root.children.size(); i++) {
        Value divisor = evaluateHelper(root.children.at(i));
        checkValidity(result, divisor, t.tokenText);
            
        if(divisor.double_value == 0.0) { // throws error if attempting to divide by zero
            zeroError();
        }
        result.double_value /= divisor.double_value;
    }
}
void InfixParser::modText(Value &result, Node &root, Token &t){
    for(unsigned int i = 1; i < root.children.size(); i++) {
        Value divisor = evaluateHelper(root.children.at(i));
        checkValidity(result, divisor, t.tokenText);
            
        if(divisor.double_value == 0.0) { // throws error if attempting to divide by zero
            zeroError(); 
        }
        result.double_value = std::fmod(result.double_value, divisor.double_value);
    }
}

void InfixParser::lessText(Value &result, Node &root, Token &t){
    for(unsigned int i = 1; i < root.children.size(); i++) {
        Value intermediate = evaluateHelper(root.children.at(i));
        checkValidity(result, intermediate, t.tokenText);
        result.type = Value::BOOL;
        result.bool_value = (result.double_value < intermediate.double_value);
    }
}

void InfixParser::lessequalText(Value &result, Node &root, Token &t){
    for(unsigned int i = 1; i < root.children.size(); i++) {
        Value intermediate = evaluateHelper(root.children.at(i));
        checkValidity(result, intermediate, t.tokenText);
        result.type = Value::BOOL;
        result.bool_value = (result.double_value <= intermediate.double_value);
    }
}

void InfixParser::greaterText(Value &result, Node &root, Token &t){
    for(unsigned int i = 1; i < root.children.size(); i++) {
        Value intermediate = evaluateHelper(root.children.at(i));
        checkValidity(result, intermediate, t.tokenText);
        result.type = Value::BOOL;
        result.bool_value = (result.double_value > intermediate.double_value);
    }
}

void InfixParser::greaterequalText(Value &result, Node &root, Token &t){
    for(unsigned int i = 1; i < root.children.size(); i++) {
        Value intermediate = evaluateHelper(root.children.at(i));
        checkValidity(result, intermediate, t.tokenText);
        result.type = Value::BOOL;
        result.bool_value = (result.double_value >= intermediate.double_value);
    }
}

void InfixParser::doubleEqual(Value& result, Node& root){
  for(unsigned int i = 1; i < root.children.size(); i++) {
        Value intermediate = evaluateHelper(root.children.at(i));
        Value::TypeTag compareType = result.type;
        result.type = Value::BOOL;

        if(result.type != intermediate.type) {
            result.bool_value = false;
            return;
        }

        if (compareType == Value::DOUBLE) {
            result.bool_value = (result.double_value == intermediate.double_value);
        }
        else if (compareType == Value::BOOL) {
            result.bool_value = (result.bool_value == intermediate.bool_value);
        }
        else {
            result.bool_value = (result.arr_value == intermediate.arr_value);
        }
   }
}

void InfixParser::notEqual(Value& result, Node& root){
    for(unsigned int i = 1; i < root.children.size(); i++) {
        Value intermediate = evaluateHelper(root.children.at(i));
        Value::TypeTag compareType = result.type;
        result.type = Value::BOOL;

        if(result.type != intermediate.type) {
            result.bool_value = false;
            return;
        }

        if (compareType == Value::DOUBLE) {
            result.bool_value = (result.double_value != intermediate.double_value);
        }
        else if (compareType == Value::BOOL) {
            result.bool_value = (result.bool_value != intermediate.bool_value);
        }
        else {
            result.bool_value = (result.arr_value != intermediate.arr_value);
        }
    }
}

void InfixParser::andText(Value& result, Node& root, Token& t){
    for(unsigned int i = 1; i < root.children.size(); i++) {
        Value intermediate = evaluateHelper(root.children.at(i));
        checkValidity(result, intermediate, t.tokenText);
        result.bool_value = (result.bool_value && intermediate.bool_value);
    }
}

void InfixParser::orText(Value& result, Node& root, Token& t){
    for(unsigned int i = 1; i < root.children.size(); i++) {
        Value intermediate = evaluateHelper(root.children.at(i));
        checkValidity(result, intermediate, t.tokenText);
        result.bool_value = (result.bool_value || intermediate.bool_value);
    }
}

void InfixParser::caretText(Value& result, Node& root, Token& t){
    for(unsigned int i = 1; i < root.children.size(); i++) {
        Value intermediate = evaluateHelper(root.children.at(i));
        checkValidity(result, intermediate, t.tokenText);
        result.bool_value = (result.bool_value != intermediate.bool_value);
    }
}

// helper functions for evaluateHelper end here.

Value InfixParser::evaluateHelper(Node root) {
    Token t = root.data;
    if (t.tokenText == "[") {
        std::vector<Value> arr = std::vector<Value>();
        for (Node n : root.children) {
            if (n.data.tokenType != ArrayIndex) {
                Value val = evaluateHelper(n);
                arr.push_back(val);
            }
        }
        Value val = Value(arr);
        Value* v = &val;
        for (Node n : root.children) { // handles array index
            if (n.data.tokenType == ArrayIndex) {
                Value arrIndex = evaluateHelper(n.children.at(0));
                if (v->type != Value::ARRAY) {
                    std::string output = "Runtime error: not an array.";
                    runTimeError(output);
                    return -123.4567;
                }
                if (arrIndex.type != Value::DOUBLE) {
                    std::string output = "Runtime error: index is not a number.";
                    runTimeError(output);
                    return -123.4567;
                }
                double filler; // used solely as parameter for modf function
                if (modf(arrIndex.double_value, &filler) != 0) {
                    std::string output = "Runtime error: index is not an integer.";
                    runTimeError(output);
                    return -123.4567;
                }
                if (arrIndex.double_value >= v->arr_value->size() || arrIndex.double_value < 0) {
                    std::string output = "Runtime error: index out of bounds.";
                    runTimeError(output);
                    return -123.4567;
                }
                v = &v->arr_value->at(arrIndex.double_value);
            }
        }
        return *v;
    }
    if(t.tokenType == Number) {// return number value
        if (root.children.size() > 0 && root.children.at(0).data.tokenType == ArrayIndex) {
            std::string output = "Runtime error: not an array.";
            runTimeError(output);
            return -123.4567;
        }
        return Value(std::stod(t.tokenText));
    }
    if(t.tokenType == Boolean) {// return boolean value
        if (root.children.size() > 0 && root.children.at(0).data.tokenType == ArrayIndex) {
            std::string output = "Runtime error: not an array.";
            runTimeError(output);
            return -123.4567;
        }
        return Value(t.tokenText == "true");
    }
    if(t.tokenType == Identifier) {
        if(variables.find(t.tokenText) == variables.end()) {// throw error if evaluating undefined variable
            std::string output = "Runtime error: unknown identifier " + t.tokenText;
            runTimeError(output);
            return -123.4567;
        }
        if (root.children.size() > 0 && root.children.at(0).data.tokenType == ArrayIndex) { // todo: make this work for multidimensional arrays
            Value arrIndex = evaluateHelper(root.children.at(0).children.at(0));
            if (variables.at(t.tokenText).type != Value::ARRAY) {
                std::string output = "Runtime error: not an array.";
                runTimeError(output);
                return -123.4567;
            } 
            if (arrIndex.type != Value::DOUBLE) {
                std::string output = "Runtime error: index is not a number.";
                runTimeError(output);
                return -123.4567;
            }
            double filler; // used solely as parameter for modf function
            if (modf(arrIndex.double_value, &filler) != 0) {
                std::string output = "Runtime error: index is not an integer.";
                runTimeError(output);
                return -123.4567;
            }
            if (arrIndex.double_value >= variables.at(t.tokenText).arr_value->size() || arrIndex.double_value < 0) {
                std::string output = "Runtime error: index out of bounds.";
                runTimeError(output);
                return -123.4567;
            }
            return variables.at(t.tokenText).arr_value->at(arrIndex.double_value);
        }
        else {
            return variables.at(t.tokenText); // return variable value
        }
    }
    
    if(root.children.size() == 0) {
        return 0.0;
    }

    if(t.tokenText == "=") {
        Value result = evaluateHelper(root.children.at(root.children.size() - 1));
        equalText(result, root);
        return result; // returns the value that all the variables were assigned to
    }

    Value result(0.0); // if this point is reached, tokenType is Operator
    result = evaluateHelper(root.children.at(0));

    diffCases(result,root,t);

    return result;
}

void InfixParser::printAll() {
    for(Node n: expressionASTs) {
        printTree(n);
        std::cout << std::endl;
        Value eval = evaluate(n);
        std::cout << eval << std::endl;
    }
}

void::InfixParser::printTree(Node root) const {
    printTreeHelper(root);
}

void InfixParser::printTreeHelper(Node root) const {
    bool op = false;
    if(root.data.tokenType == Operator || root.data.tokenType == Assignment || root.data.tokenType == LogicOperator) {
        op = true;
    }

    // prints parenthesis for each operator
    if(op) {
        std::cout << "(";
    }

    if (root.data.tokenText == "[") { // print out an array
        std::cout << "[";
        for(unsigned int i = 0; i < root.children.size(); i++) {
            Node n = root.children.at(i);
            if (n.data.tokenType == ArrayIndex) {
                continue;
            }
            if(i == 0) {
                printTreeHelper(n);
            }
            else {
                std::cout << ", ";
                printTreeHelper(n);
            }
        }
        std::cout << "]";
        for(unsigned int i = 0; i < root.children.size(); i++) { // loop again to print array indices if needed
            Node n = root.children.at(i);
            if (n.data.tokenType == ArrayIndex) {
                std::cout << "[";
                printTreeHelper(root.children.at(i).children.at(0)); // print index
                std::cout << "]";
            }
        }
    }
    else {
        if(root.data.tokenType == Number) {
            std::cout << std::stod(root.data.tokenText);
        }
        else if(root.data.tokenType == Identifier) {
            std::cout << root.data.tokenText;
        }
        else if (root.data.tokenType == Boolean) {
            std::cout << root.data.tokenText;
        }

        // prints the operator between the children
        for(unsigned int i = 0; i < root.children.size(); i++) {
            Node n = root.children.at(i);
            if (root.children.at(i).data.tokenType == ArrayIndex) {
                // print out an array index
                std::cout << "[";
                printTreeHelper(root.children.at(i).children.at(0)); // print index
                std::cout << "]";
            }
            else if (i == 0) {
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
}

unsigned int getPrecedence(std::string op) {// self explanatory
    if (op == "*" || op == "/" || op == "%") {
        return 8;
    }
    else if (op == "+" || op == "-")
    {
        return 7;
    }
    else if (op == "<" || op == "<=" || op == ">" || op == ">=")
    {
        return 6;
    }
    else if (op == "==" || op == "!=")
    {
        return 5;
    }
    else if (op == "&")
    {
        return 4;
    }
    else if (op == "^")
    {
        return 3;
    }
    else if (op == "|")
    {
        return 2;
    }
    else if(op == "=") {// unused because "=" is right associative
        return 1;
    }

    return 0;
}

void InfixParser::fillTreeInfix(std::vector<Token>& lexed) {
    if(lexed.size() == 0){// case should not happen
        return;
    }
    
    if(lexed.size() == 1) {// throws error if there is only one token (likely End token)
        Token t = lexed.at(0);
        unexpectedTokenError(t);
    }

    unsigned int index = 0;
    root = fillTreeSubexpression(lexed, index);

    if(parenCounter != 0) {// throw error if parentheses are unbalanced
        unexpectedTokenError(lexed.at(lexed.size() - 1));
    }

    expressionASTs.push_back(root);
}

// Add stuff for assignment operator once Lexer is updated
InfixParser::Node InfixParser::fillTreeSubexpression(std::vector<Token>& lexed, unsigned int& index) {
    Node lhs = Node(Token());
    Token t1 = lexed.at(index);
    if(t1.tokenType == Number || t1.tokenType == Identifier || t1.tokenType == Boolean) {
        lhs = Node(t1);
        Token t2 = lexed.at(++index);
        if(t2.tokenType == End) {
            // if the first token is a number and the second is End, return because there is nothing
            // more to do
            return lhs;
        }
        else if(t2.tokenText == ")") { // end the current subtree when reaching ")"
            if(parenCounter == 0) {// throw error if ")" outnumbers "(" 
                unexpectedTokenError(t2);
            }
            parenCounter--;
            return lhs;
        }
        else if (t2.tokenText == ",") { // end subtree when reaching comma
            return lhs;
        }
        else if (t2.tokenText == "]") { // end subtree when reaching right bracket
            index--;
            return lhs;
        }
    }
    else if(t1.tokenText == "(") {// treat everything inside parenthesis as its own subtree/subexpression
        parenCounter++;
        index++;
        lhs = fillTreeSubexpression(lexed, index);
        if(lhs.children.size() == 0) {
            return lhs;
        }
    }
    else if(t1.tokenText == "[") { // array
        // std::cout << "BUILDING ARRAY" << std::endl;
        lhs = buildArray(lexed, index);
    }
    else {
        unexpectedTokenError(t1);
    }

    while(index < lexed.size()) {
        Token t = lexed.at(index);

        if(t.tokenType == End) {
            break;
        }
        else if(t.tokenText == ")") {// end the current subtree when reaching ")"
            index++;
            if(parenCounter == 0) {// throw error if ")" outnumbers "(" 
                unexpectedTokenError(t);
            }
            parenCounter--;
            break;
        }
        else if(t.tokenText == ",") {// end the current subtree when reaching ","
            // unexpectedTokenError(lexed.at(index));
            break;
        }
        else if(t.tokenText == "]") {// end the current subtree when reaching "]"
            index--;
            break;
        }
        else if (t.tokenText == "[") { // array index
            // std::cout << "ARRAY INDEX" << std::endl;
            Node arrIndex = Node(Token(t.lineNumber, t.columnNumber, t.tokenText, ArrayIndex));
            index++;
            Node indExpression = fillTreeSubexpression(lexed, index);
            arrIndex.children.push_back(indExpression);
            lhs.children.push_back(arrIndex); // makes first child/children of the Identifier node ArrayIndex type(s)
            if (lexed.at(index).tokenType == Comma) { // array format in an array index
                unexpectedTokenError(lexed.at(index));
            }
            while (index < lexed.size() && lexed.at(index).tokenText != "]") {
                index++;
            }
            index++; // moves index after closed brackets
        }
        else if(t.tokenType == Operator || t.tokenType == Assignment || t.tokenType == LogicOperator) {
            Node opNode = Node(t); // new head of the tree/subtree 
            opNode.children.push_back(lhs); // current tree becomes the LHS of new head

            Node rhs;
            index++;
            int currParenCounter = parenCounter;
            if (t.tokenType == Operator) {// right hand side is everything with greater precedence
                rhs = fillTreeInfixHelper(lexed, index, getPrecedence(t.tokenText)); 
            }
            else if (t.tokenType == LogicOperator) {
                rhs = fillTreeInfixHelper(lexed, index, getPrecedence(t.tokenText)); 
            }
            else {// assignment operator, right hand side is the remaining subexpression
                rhs = fillTreeSubexpression(lexed, index);
            }
            opNode.children.push_back(rhs);
            lhs = opNode; 

            if(t.tokenType == Assignment) {
                index++;
                return lhs;
            }

            // if parenCounter is less than before, break because it is not a top-level expression
            if(currParenCounter > parenCounter) {
                break;
            }
        }
        else {
            unexpectedTokenError(t);
        }
    }
    return lhs;
}

InfixParser::Node InfixParser::fillTreeInfixHelper(std::vector<Token>& lexed, unsigned int& index, unsigned int currPrecedence) {
    Node lhs;
    Token t = lexed.at(index); // either a number, variable, or subexpression
    if(t.tokenType == Number || t.tokenType == Identifier || t.tokenType == Boolean) {
        lhs = Node(t);
    }
    else if(t.tokenText == "(") {// treat everything inside parethesis as its own subtree/subexpression
        parenCounter++;
        index++;
        lhs = fillTreeSubexpression(lexed, index);
        index--;
    }
    else if(t.tokenText == "[") { // array
        lhs = fillTreeSubexpression(lexed, index);
    }
    else {
        unexpectedTokenError(t);
    }

    while(index < lexed.size() - 1) {
        Token t2 = lexed.at(++index); // should be an operator or ")" or End token
        if(t2.tokenType == End || t2.tokenType == Assignment) {
            return lhs;
        }
        else if(t2.tokenText == ")") {// end the current subtree when reaching ")"
            if(parenCounter == 0) {
                unexpectedTokenError(t2);
            }
            parenCounter--;
            index++;
            return lhs;
        }
        else if(t2.tokenText == ",") {// end the current subtree when reaching ","
            return lhs;
        }
        else if(t2.tokenText == "]") {// end the current subtree when reaching "]"
            return lhs;
        }
        else if (t2.tokenText == "[") { // array index
            Node arrIndex = Node(Token(t2.lineNumber, t2.columnNumber, t2.tokenText, ArrayIndex));
            index++;
            Node indExpression = fillTreeSubexpression(lexed, index);
            arrIndex.children.push_back(indExpression);
            lhs.children.push_back(arrIndex); // makes first child/children of the Identifier node ArrayIndex type(s)
            if (lexed.at(index).tokenType == Comma) { // array format in an array index
                unexpectedTokenError(lexed.at(index));
            }
            while (index < lexed.size() && lexed.at(index).tokenText != "]") {
                index++;
            }
            index++; // moves index after closed brackets
        }
        else if(t2.tokenType != Operator && t2.tokenType != LogicOperator) {
            unexpectedTokenError(t2);
        }

        if(getPrecedence(t2.tokenText) <= currPrecedence) {// returns when operator precedence is not > original precedence
            return lhs;
        }
        
        Node opNode = Node(t2); // new head of the tree/subtree 
        opNode.children.push_back(lhs); // current tree becomes the LHS of new head
        // rhs becomes everything to the immediate right with greater precedence
        Node rhs = fillTreeInfixHelper(lexed, ++index, getPrecedence(t2.tokenText)); 
        index--;
        opNode.children.push_back(rhs);
        lhs = opNode;
    }  

    return lhs;
}

InfixParser::Node InfixParser::buildArray(std::vector<Token>& lexed, unsigned int& index) {
    Token t = lexed.at(index); // should be left square bracket
    Node arrHead = Node(t);
    while(index < lexed.size() - 1) {
        Token t2 = lexed.at(++index);
        if(t2.tokenType == End || t2.tokenType == Assignment) {
            return arrHead;
        }
        else if(t2.tokenType == Comma) {
            continue;
        }
        else if (t2.tokenText == "]") {
            index++;
            return arrHead;
        }
        else {
            Node child = fillTreeSubexpression(lexed, index);
            arrHead.children.push_back(child); // adds all the elements of the array as a child to the left bracket node
        }
    }
    return arrHead;
}