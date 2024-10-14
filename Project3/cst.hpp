#ifndef CSTREE_HPP
#define CSTREE_HPP

#include <iostream>
#include <vector>

#include "token.hpp"
#include "token_enum.hpp"
#include "token_node.hpp"

class CSTree {
   public:
    CSTree(std::vector<Token>& tokens);
    TokenNode* head() { return _head; }

   private:
    TokenNode* _head;

    TokenNode* _previous;
    TokenNode* _current;

    std::vector<Token>::iterator _nIt;

   private:
    void isFor();

    bool isInitializationExpression();
    bool isBooleanExpression();
    bool isNumericalExpression();
    bool isExpression();

   private:
    bool isOperand(TokenNode* token);

   private:
    bool _operandFlag;
};

#endif  // CSTREE_HPP