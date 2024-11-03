#ifndef ASTREE_HPP
#define ASTREE_HPP

#include "ast_list_node.hpp"
#include "cst.hpp"
#include "symbol_table.hpp"
#include "token.hpp"
#include "token_enum.hpp"
#include "token_node.hpp"

class ASTree {
   public:
    ASTree(CSTree *cTree, SymbolTable *symTable);

    ASTListNode *head() { return _head; }
    ASTListNode *tail() { return _tail; }

   private:
    ASTListNode *_head{nullptr};
    ASTListNode *_tail{nullptr};

    ASTListNode *_previous{nullptr};
    ASTListNode *_current{nullptr};
    TokenNode *_currCNode;

    SymbolTable *symTable;
    CSTree *cTree;

   private:
    void addNext(ASTListNode *next);
    void advance();
    bool isDelimiter(TokenType type);

    ASTListNode *parseDeclaration();
};

#endif  // ASTREE_HPP
