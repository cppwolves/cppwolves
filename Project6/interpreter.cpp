#include "interpreter.hpp"

#include <iostream>

Interpreter::Interpreter(ASTree* ast, SymbolTable* symTable) : ast(ast), symbolTable(symTable) {
    current = ast->head();

    while (current) {
        std::cout << current->lexeme;
        if (current->token) {
            std::cout << " - " << current->token->lexeme;
        }
        std::cout << std::endl;
        advance();
    }
    std::cout << "interpreter done" << std::endl;
}

void Interpreter::advance() {
    // std::cout << "a " << _currCNode->lexeme << std::endl;
    if (current->sibling) {
        current = current->sibling;
    } else if (current->child) {
        current = current->child;
    } else {
        current = nullptr;
    }
    return;
}