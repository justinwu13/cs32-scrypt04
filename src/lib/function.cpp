// function.cpp
// implement Function class

#include <vector>
#include <string>
#include "token.h"
#include "statementnode.h"
#include "function.h"

Function::Function(std::string name, int indentation) {
    this->name = name;
    this->initialIndentation = indentation;
}

Function::Function(std::string name, int indentation, std::vector<StatementNode> instructions, std::vector<std::string> params) {
    this->name = name;
    this->initialIndentation = indentation;
    this->procedure = instructions;
    this->parameters = params;
}

std::string Function::getName() {
    return name;
}