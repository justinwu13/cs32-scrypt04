// statementparser.cpp
// implement StatementParser class

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "token.h"
#include "value.h"
#include "infixparser.h"
#include "statementparser.h"
#include "function.h"

StatementParser::StatementParser() {
    braceCounter = 0;
    par = InfixParser();
    statements = std::vector<StatementNode>();
}

void StatementParser::printProcedure() {
    for(StatementNode& s: procedure) {
        printProcedureHelper(s);
    }
}

void StatementParser::printProcedureHelper(StatementNode& statement) {

    for(int i = 0; i < statement.indentationLevel; i++) {
        std::cout << "    ";
    }    

    Token t = statement.statementToken;
    if(t.tokenType == Funcdef) {
        std::cout << "def ";
    }
    std::cout << t.tokenText;
    if(t.tokenType == Funcdef) {
        std::cout << "(";
        for(unsigned int i = 0; i < statement.params.size(); i++) {
            std::cout << statement.params.at(i);
            if(i != statement.params.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << ")";
    }
    // whitespace, otherwise would end up with output like "printsteps" or "ifx > 1 {"
    if(t.tokenType == Statement && t.tokenText != "else" && !(t.tokenText == "return" && statement.infixExpression.data.tokenType == Undefined)) {
        std::cout << " ";
    }
    par.printTree(statement.infixExpression);
    // all statements except for print and return have their own set of curly brackets
    if((t.tokenType == Statement && t.tokenText != "print" && t.tokenText != "return") || t.tokenType == Funcdef) {
        std::cout << " {";
    }
    // all bare expressions, print statements, and return statements have semicolon 
    if((t.tokenType != Statement && t.tokenType != Funcdef) || t.tokenText == "print" || t.tokenText == "return") {
        std::cout << ";";
    }
    std::cout << std::endl;
    
    // recursively print the statements stored in each block 
    for(StatementNode& s: statement.children) {
        printProcedureHelper(s);
    }

    if((t.tokenType == Statement && t.tokenText != "print" && t.tokenText != "return") || t.tokenType == Funcdef) {
        for(int i = 0; i < statement.indentationLevel; i++) {
            std::cout << "    ";
        } 
        std::cout << "}" << std::endl;
    }
}

// only reads/stores input, does not organize into blocks yet 
void StatementParser::readStatements(std::vector<Token>& lexed) {
    if(lexed.size() <= 1) {
        return;
    }

    unsigned int index = 0;
    while(index < lexed.size()) {
        par.clearAST();
        Token first = lexed.at(index);
        StatementNode statement;
        statement.statementToken = first;
        if(first.tokenText == "return" || first.tokenText == "print") {
            if(first.tokenText == "return" && lexed.at(index + 1).tokenText == ";") {
                index++;
            }
            else {
                statement.infixExpression = readExpression(lexed, ++index, ";");
            }
        }
        else if(first.tokenType == Funcdef) {
            statements.push_back(statement);
            StatementNode funcName = StatementNode(lexed.at(++index));
            statements.push_back(funcName);
            if(funcName.statementToken.tokenType != Identifier) {
                par.unexpectedTokenError(funcName.statementToken);
            }
            if(lexed.at(++index).tokenText != "(") {
                par.unexpectedTokenError(lexed.at(index));
            }

            if(lexed.at(index + 1).tokenText != ")") {
                while(true) { // break or error should always be hit eventually
                    Token varToken = lexed.at(++index);
                    if(varToken.tokenType != Identifier) {
                        par.unexpectedTokenError(varToken);
                    }
                    statements.push_back(StatementNode(varToken));
                    Token separator = lexed.at(++index);
                    if(separator.tokenText == ")") {
                        break;
                    }
                    else if(separator.tokenType != Comma) {
                        par.unexpectedTokenError(separator);
                    }
                }
            }
            else {
                index++;
            }
            if(lexed.at(++index).tokenText != "{") {
                par.unexpectedTokenError(lexed.at(index));
            }
            index++;
            continue;
        }
        else if(first.tokenType == Statement) {
            if(first.tokenText == "else") {
                Token second = lexed.at(index + 1);
                if(second.tokenText == "if") {
                    statement.statementToken.tokenText = "elif";
                }
                else if(second.tokenText != "{") {
                    par.unexpectedTokenError(second);
                }
                else {
                    index++;
                }
            }
            else { //if or while
                statement.infixExpression = readExpression(lexed, ++index, "{");
            }
        }
        else if(first.tokenText == "}") {
            
        }
        else if(first.tokenType == End) {
            break;
        }
        else { // bare expression
            statement.statementToken.tokenText = "";
            statement.infixExpression = readExpression(lexed, index, ";");
        }
        index++;
        statements.push_back(statement);
    }
}

Node StatementParser::readExpression(std::vector<Token>& lexed, unsigned int& index, std::string delimiter) {    
    std::vector<Token> subexpression;
    while(index < lexed.size()) {
        Token t = lexed.at(index);
        if(t.tokenText == delimiter) {
            Token end = t;
            t.tokenType = End;
            subexpression.push_back(t);
            break;
        }
        else if(t.tokenType == End) { // will throw error in fillTreeSubExpression
            subexpression.push_back(t);
            break;
        }

        subexpression.push_back(t);
        index++;
    }

    unsigned int subIndex = 0;
    return par.fillTreeSubexpression(subexpression, subIndex);
}

// organizes statements into blocks
void StatementParser::fillProcedure() {
    unsigned int index = 0; // index can be updated by every instance of fillSubProcedure as it is passed by reference
    fillSubProcedure(procedure, index, 0); 
}

void StatementParser::fillSubProcedure(std::vector<StatementNode>& currBlock, unsigned int& index, int indentation) {
    while(index < statements.size()) {
        StatementNode s = statements.at(index);
        s.indentationLevel = indentation;

        if(s.statementToken.tokenType == Statement && s.statementToken.tokenText != "print" && s.statementToken.tokenText != "return") {
            if(s.statementToken.tokenText == "elif") {
                s.statementToken.tokenText = "else"; // changes elif back to else for proper output
                fillElif(s.children, ++index, indentation + 1); // special call for else if
            }
            else { // while, if, and else blocks fill recursively, new "currBlock" is their children vector 
                fillSubProcedure(s.children, ++index, indentation + 1); 
                braceCounter++;
            }
            currBlock.push_back(s);
        }
        else if(s.statementToken.tokenText == "}") { // end current block if "}" encountered
            if(indentation == 0) {
                par.unexpectedTokenError(s.statementToken);
            }
            s.indentationLevel--;
            braceCounter--;
            return;
        }
        else if(s.statementToken.tokenType == Funcdef) {
            fillFunc(s, ++index, indentation);
            currBlock.push_back(s);
        }
        else {
            currBlock.push_back(s);
        }

        index++;
    }
}

// helper function specific for else if case
// only gets the following if block as well as possible else block 
void StatementParser::fillElif(std::vector<StatementNode>& currBlock, unsigned int& index, int indentation) {
    StatementNode ifNode = statements.at(index); // should be an if statement
    ifNode.indentationLevel = indentation;
    fillSubProcedure(ifNode.children, ++index, indentation + 1);
    currBlock.push_back(ifNode);
    index++;
    
    if(index >= statements.size()) {
        return;
    }    

    StatementNode elseNode = statements.at(index); // check for an else after the if
    elseNode.indentationLevel = indentation;
    if(elseNode.statementToken.tokenText != "else") { // stop looking if "else" not found
        index--;
        return;
    }
    else { // "else" found
        fillSubProcedure(elseNode.children, ++index, indentation + 1);
        currBlock.push_back(elseNode);
    }
}

// helper function to define functions
void StatementParser::fillFunc(StatementNode& s, unsigned int& index, int indentation) {
    std::string funcName = statements.at(index).statementToken.tokenText;
    s.statementToken.tokenText = funcName;
    index++;
    while(true) { // break or error should be hit eventually
        if(index >= statements.size()) { // should not happen
            break;
        }
        Token currToken = statements.at(index).statementToken;
        if(currToken.tokenType == Identifier) {
            s.params.push_back(currToken.tokenText);
        }
        else {
            break;
        }
        index++;
    }
    fillSubProcedure(s.children, index, indentation + 1);
}

std::vector<StatementNode> StatementParser::getProcedure() {
    return procedure;
}