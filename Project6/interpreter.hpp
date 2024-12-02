
#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "ast.hpp"
#include "ast_list_node.hpp"
#include "symbol_table.hpp"

class Interpreter {
   public:
    Interpreter(ASTree* ast, SymbolTable* symTable);
    ASTListNode* getAddressAtInd(int index);
    ASTListNode* getMain();

   private:
    ASTree* ast;
    SymbolTable* symbolTable;

    ASTListNode* current;  // pc?
    ASTListNode* main;
    std::vector<ASTListNode*> _addresses;

   private:
    void advanceAST();
    void advanceAddress();

    // testing
   private:
    void printAddresses();
};

#endif  // INTERPRETER_HPP
