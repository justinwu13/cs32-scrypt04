// node.h

#pragma once
#include "token.h"
#include <vector>

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