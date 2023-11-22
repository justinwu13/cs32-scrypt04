// function.h

#pragma once
#include <vector>
#include <string>
#include <map>
#include "token.h"
#include "statementnode.h"

class Function {
    public:
        Function(std::string name, int indentation);
        Function(std::string name, int indentation, std::vector<StatementNode> instructions, std::vector<std::string> params);
        std::vector<std::string> parameters;
        std::vector<StatementNode> procedure;
        std::string getName();

    private:
        std::string name;
        int initialIndentation;
};