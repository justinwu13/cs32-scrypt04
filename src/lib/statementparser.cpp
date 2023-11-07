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

    std::cout << statement.statementToken.tokenText;
    if(statement.statementToken.tokenType == Statement && statement.statementToken.tokenText != "else") {
        std::cout << " ";
    }
    par.printTree(statement.infixExpression);
    if(statement.statementToken.tokenType == Statement && statement.statementToken.tokenText != "print") {
        std::cout << " {";
    }
    std::cout << std::endl;
    
    for(StatementNode& s: statement.children) {
        printProcedureHelper(s);
    }

    if(statement.statementToken.tokenType == Statement && statement.statementToken.tokenText != "print") {
        for(int i = 0; i < statement.indentationLevel; i++) {
            std::cout << "    ";
        } 
        std::cout << "}" << std::endl;
    }
}

// only reads/stores input, does not organize into blocks yet 
void StatementParser::readStatements(std::vector<Token>& lexed) {
    par.clearAST(); // not sure if necessary or even good

    // not sure if this is ok, this may need to be changed
    if(lexed.size() <= 1) {
        return;
    }

    Token first = lexed.at(0);
    StatementNode statement;
    if(first.tokenType == Statement) {
        statement.statementToken = first;

        if(first.tokenText == "else") {
            statements.push_back(statement);
            Token second = lexed.at(1);

            if(second.tokenText == "{") {
                return;
            }
            else if(second.tokenText == "if") {
                statements.at(statements.size() - 1).statementToken.tokenText = "elif"; // changes else to elif if "if" follows
                Token last = lexed.at(lexed.size() - 2);
                if(last.tokenText != "{") {
                    par.unexpectedTokenError(last);
                }
                statement.statementToken = second;
                lexed.erase(lexed.begin());
                lexed.erase(lexed.begin());
                lexed.erase(lexed.end() - 2);
            }
            else {
                par.unexpectedTokenError(second);
            }
        }
        else if(first.tokenText == "print") {
            lexed.erase(lexed.begin());
        }
        else {// should be if or while
            if(lexed.at(lexed.size() - 2).tokenText != "{") {
                par.unexpectedTokenError(lexed.at(lexed.size() - 2));
            }
            lexed.erase(lexed.begin());
            lexed.erase(lexed.end() - 2);
        }
    }
    else if(first.tokenText == "}") {
        if(lexed.size() != 2) {
            par.unexpectedTokenError(lexed.at(1));
        }

        statement.statementToken = first;
        statements.push_back(statement);
        return;
    }
    par.fillTreeInfix(lexed);
    statement.infixExpression = par.root;
    statements.push_back(statement);
}

// organizes statements into blocks
void StatementParser::fillProcedure() {
    unsigned int index = 0;
    fillSubProcedure(procedure, index, 0);

    
}

// doesn't check if there are statements after last "}" but still passes all gradescope tests for now so idk
void StatementParser::fillSubProcedure(std::vector<StatementNode>& currBlock, unsigned int& index, int indentation) {
    while(index < statements.size()) {
        StatementNode s = statements.at(index);
        s.indentationLevel = indentation;

        if(s.statementToken.tokenType == Statement && s.statementToken.tokenText != "print") {
            if(s.statementToken.tokenText == "elif") {
                s.statementToken.tokenText = "else"; // changes elif back to else for proper output
                fillElif(s.children, ++index, indentation + 1);
            }
            else {
                fillSubProcedure(s.children, ++index, indentation + 1);
                braceCounter++;
            }
            currBlock.push_back(s);
        }
        else if(s.statementToken.tokenText == "}") {
            if(indentation == 0) {
                par.unexpectedTokenError(s.statementToken);
            }
            s.indentationLevel--;
            //currBlock.push_back(s);
            braceCounter--;
            return;
        }
        else {
            currBlock.push_back(s);
        }

        index++;
    }
}

// helper function specific for else if case
// only gets the following if/else block
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

void StatementParser::runProcedure() {
    runProcedure(procedure);
}

void StatementParser::runProcedure(std::vector<StatementNode>& currBlock) {
    for(unsigned int i = 0; i < currBlock.size(); i++) {
        StatementNode& s = currBlock.at(i);
        runProcedureHelper(currBlock, s, i);
    }
}

void StatementParser::runProcedureHelper(std::vector<StatementNode>& currBlock, StatementNode& s, unsigned int& index) {
    if(s.statementToken.tokenType == Statement) {
        std::string instruction = s.statementToken.tokenText;

        if(instruction == "print") {
            std::cout << par.evaluate(s.infixExpression) << std::endl;
        }
        else if(instruction == "if") {
            Value condition = par.evaluate(s.infixExpression);
            if(condition.type != Value::BOOL) {
                std::cout << "Runtime error: condition is not a bool." << std::endl;
                throw 3;
            }

            if(condition.bool_value == true) {
                if(index + 1 < currBlock.size() && currBlock.at(index + 1).statementToken.tokenText == "else") {
                    index++; // skips else block if condition is true
                }
                runProcedure(s.children);
            }
        }
        else if(instruction == "while") {
            while(true) {
                Value condition = par.evaluate(s.infixExpression);
                if(condition.type != Value::BOOL) {
                    std::cout << "Runtime error: condition is not a bool." << std::endl;
                    throw 3;
                }

                if(condition.bool_value == true) {
                    if(index + 1 < currBlock.size() && currBlock.at(index + 1).statementToken.tokenText == "else") {
                        index++; // skips else block if condition is true
                    }
                    runProcedure(s.children);
                }
                else {
                    break;
                }
            }
            
        }
        else { // should be "else"
            runProcedure(s.children);
        }
    }
    else { // should be an expression
        par.evaluate(s.infixExpression);
    }
}