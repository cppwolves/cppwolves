#include "interpreter.hpp"

#include <iostream>

Interpreter::Interpreter(ASTree* ast, SymbolTable* symTable) : ast(ast), symbolTable(symTable), main(nullptr), current(nullptr) {
    current = ast->head();
    size_t addressIdx = 0;  // not needed to store as member? can access with .size()

    // address assignment
    while (current) {
        if (current->type != ASTNodeType::BEGIN_BLOCK && current->type != ASTNodeType::END_BLOCK) {
            // set symbolNode's address
            if (current->type == ASTNodeType::DECLARATION) {
                current->symbol->address = addressIdx;

                // set program start point
                if (current->token->type == TokenType::MAIN) {
                    main = current;
                }
            }
            _addresses.push_back(current);
            addressIdx++;
        }
        advanceAddress();
    }

    //printAddresses();
}

void Interpreter::printAddresses() {
    for (size_t i = 0; i < _addresses.size(); i++) {
        std::cout << i << " " << _addresses[i]->lexeme;
        if (_addresses[i]->token) {
            std::cout << " - " << _addresses[i]->token->lexeme;
        }
        std::cout << std::endl;
    }
}

// for traversing single node of AST at a time
void Interpreter::advanceAST() {
    if (current->sibling) {
        current = current->sibling;
    } else if (current->child) {
        current = current->child;
    } else {
        current = nullptr;
    }
    return;
}

// for traversing single row of AST at a time
void Interpreter::advanceAddress() {
    while (current->sibling) {
        current = current->sibling;
    }
    current = current->child ? current->child : nullptr;
    return;
}


ASTListNode* Interpreter::getAddressAtInd(int index) {
    return _addresses.at(index);
}

ASTListNode* Interpreter::getMain() {
    return main;
}
