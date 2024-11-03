#include "ast.hpp"

#include <iostream>

ASTree::ASTree(CSTree* cTree, SymbolTable* symTable) : cTree(cTree), symTable(symTable), _current(nullptr) {
    // ast node: _current
    // cst node: _currCNode
    _currCNode = cTree->head();

    int braceCount = 0;
    while (_currCNode) {
        // std::cout << "c: " << _currCNode->lexeme << std::endl;
        switch (_currCNode->type) {
            case TokenType::FUNCTION:
            case TokenType::PROCEDURE:
            case TokenType::INT:
            case TokenType::CHAR:
            case TokenType::BOOL: {
                // handle declaration
                addNext(parseDeclaration());
                break;
            }
            case TokenType::IF:
            case TokenType::ELSE:
            case TokenType::WHILE: {
                advance();

                // handle postfix bool exp
                break;
            }
            case TokenType::FOR: {
                advance();

                // handle FOR (break into 3)
                break;
            }
            case TokenType::PRINTF: {
                advance();

                // handle printf
                break;
            }
            case TokenType::RETURN: {
                advance();

                // handle return ?
                break;
            }
            case TokenType::IDENTIFIER: {
                advance();

                // handle identifier (+list) for postfix exp
                break;
            }
            case TokenType::L_BRACE: {
                // handle begin block
                addNext(new ASTListNode(ASTNodeType::BEGIN_BLOCK));
                advance();
                break;
            }
            case TokenType::R_BRACE: {
                // handle end block
                addNext(new ASTListNode(ASTNodeType::END_BLOCK));
                advance();
                break;
            }
            default: {
                advance();
                // error?
                // not sure what would go here, of if anything can
            }
        }
    }
}

// adding child directly down currently, should be added to the sib of current
void ASTree::addNext(ASTListNode* node) {
    if (!_head) {
        _head = node;
        _current = _head;
    } else {
        _current->child = node;
        _current = _current->child;
    }
}

ASTListNode* ASTree::parseDeclaration() {
    ASTListNode* node = new ASTListNode(ASTNodeType::DECLARATION);

    // reach identifer, should only be max twice (function + type + [name])
    while (_currCNode->type != TokenType::IDENTIFIER) {
        advance();
    }

    node->symbol = symTable->find(_currCNode->lexeme);
    while (!isDelimiter(_currCNode->type)) {
        // std::cout << _currCNode->lexeme << std::endl;
        advance();
    }

    // std::cout << node->symbol->identifierName << " " << node->symbol->scope << std::endl;
    return node;
}

// clean up after, leave in blocks for needed debugging
void ASTree::advance() {
    if (_currCNode->sibling) {
        _currCNode = _currCNode->sibling;
    } else if (_currCNode->child) {
        _currCNode = _currCNode->child;
    } else {
        _currCNode = nullptr;
    }
    return;
}

// for block delimiters, not handling multiline strings
bool ASTree::isDelimiter(TokenType type) {
    return (type == TokenType::L_BRACE ||
            type == TokenType::R_BRACE ||
            type == TokenType::SEMICOLON);
}