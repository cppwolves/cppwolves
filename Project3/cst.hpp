#ifndef CSTREE_HPP
#define CSTREE_HPP

#include <stack>
#include <vector>

#include "token.hpp"
#include "token_enum.hpp"
#include "token_node.hpp"

class CSTree {
   public:
    CSTree(std::vector<Token> &tokens);
    TokenNode *head() { return _head; }

   private:
    TokenNode *_head;

    TokenNode *_previous;
    TokenNode *_current;

    std::vector<Token>::iterator _nIt;

   private:
    void isFor();
    void isWhile();
    void isFunction();
    void isDatatypeSpecifier();
    void isProcedure();
    void isMain();
    void addSiblingAndAdvance(TokenNode *node);
    void addChildAndAdvance(TokenNode *node);

    bool isInitializationExpression();
    bool isBooleanExpression();
    bool isNumericalExpression();
    bool isExpression();
    bool isParameterList();

   private:
    void handleOpenCloseDelimiters(TokenNode *node);
    bool isOperand(TokenNode *token);
    void throwTokenError(TokenNode *node, const std::string &message);
    void revertState(TokenNode *node);

   private:
    bool _operandFlag;
    bool _chainCheck;
    bool _paramCheck;
    std::stack<TokenType> _openStack{};
};

#endif  // CSTREE_HPP
