#include "cst.hpp"

CSTree::CSTree(std::vector<Token> &tokens) {
    if (tokens.empty()) {
        // return nullptr;
        // deal with, what behavior?
    }

    TokenNode *head = new TokenNode(tokens.at(0));
    TokenNode *curr = head;
    _head = head;  // fix this mess

    _current = head;
    _nIt = tokens.begin();

    for (_nIt++; _nIt != tokens.end(); _nIt++) {
        TokenNode *next = new TokenNode(*_nIt);
        if (
            // The next token is a delimiter
            (next->type == TokenType::L_BRACE || next->type == TokenType::R_BRACE || next->type == TokenType::ELSE) ||
            // The current token is a delimiter
            (curr->type == TokenType::ELSE || curr->type == TokenType::L_BRACE || curr->type == TokenType::SEMICOLON)) {
            // Start a new row
            curr->child = next;
            _current = next;

        } else {
            // Append to current row
            curr->sibling = next;
            _current = next;

            switch (next->type) {
                case TokenType::FOR: {
                    isFor();
                    break;
                }
                default:
                    curr->sibling = next;
                    break;
            }
        }
        curr = (_current != next) ? _current : next;
    }
}

void CSTree::isFor() {
    try {
        TokenNode *lParen = new TokenNode(*_nIt++);
        if (lParen->type != TokenType::L_PAREN) {
            throw std::runtime_error("Missing L-Paren");
        }
        _current->sibling = lParen;
        _current = lParen;

        if (!isInitializationExpression()) {
            throw std::runtime_error("Missing initialization expression");
        }

        return;
        // haven't worked out the rest yet

        TokenNode *semiColon = new TokenNode(*_nIt++);
        if (semiColon->type != TokenType::SEMICOLON) {
            throw std::runtime_error("Missing semicolon");
        }
        _current->sibling = semiColon;
        _current = semiColon;

        if (!isBooleanExpression()) {
            throw std::runtime_error("Missing boolean expression");
        }

        TokenNode *semiColon2 = new TokenNode(*_nIt++);
        if (semiColon2->type != TokenType::SEMICOLON) {
            throw std::runtime_error("Missing semicolon");
        }
        _current->sibling = semiColon2;
        _current = semiColon2;

        if (!isExpression()) {
            throw std::runtime_error("Missing expression");
        }

        TokenNode *rParen = new TokenNode(*_nIt++);
        if (rParen->type != TokenType::R_PAREN) {
            throw std::runtime_error("Missing R-Paren");
        }
        _current->sibling = rParen;
        _current = rParen;

        _nIt++;
    } catch (const std::exception &ex) {
        std::cout << "malformed for loop: "
                  << ex.what() << std::endl;
        exit(1);
    }
}

bool CSTree::isInitializationExpression() {
    try {
        // needs to handle (INT, CHAR, DATATYPE, etc before checking identifier)
        // currently, does not
        // should be before called? check BNF

        // must start with identifier
        TokenNode *identifier = new TokenNode(*_nIt++);
        if (identifier->getTypeName() != "IDENTIFIER") {
            throw std::runtime_error("Missing indentifier");
        }

        _current->sibling = identifier;
        _current = identifier;

        // followed by assignment operator
        TokenNode *assignmentOp = new TokenNode(*_nIt++);
        if (assignmentOp->type != TokenType::ASSIGNMENT_OPERATOR) {
            throw std::runtime_error("Missing assignment operator");
        }

        _current->sibling = assignmentOp;
        _current = assignmentOp;

        // can be string OR expression
        TokenNode *unknown = new TokenNode(*_nIt++);
        if (unknown->type == TokenType::STRING) {
            _current->sibling = unknown;
            _current = unknown;
            return true;
        }

        // else, must be an expression to be valid

        // isExpression(); create
        return true;

    } catch (const std::exception &ex) {
        std::cout << "malformed initialization expression: "
                  << ex.what() << std::endl;
        exit(1);
    }
}

bool CSTree::isBooleanExpression() {
    return true;  // placeholder
}

bool CSTree::isExpression() {
    // valid if boolean or numerical expression
    // account for recursion

    // boolean expression

    // numerical expression

    return true;  // placeholder
}
