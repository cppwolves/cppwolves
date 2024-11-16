
#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "ast.hpp"
#include "ast_list_node.hpp"
#include "symbol_table.hpp"

class Interpreter {
   public:
    Interpreter(ASTree& ast, SymbolTable& symTable);

   private:
    ASTree ast;
    SymbolTable symbolTable;

   private:
    ASTListNode* current;  // pc?
    std::vector<ASTListNode*> _addresses;
    void advance();
};

#endif  // INTERPRETER_HPP
