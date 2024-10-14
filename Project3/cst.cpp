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
            switch (curr->type) {
                case TokenType::FOR: {
                    isFor();
                    break;
                }
                default:
                    curr->sibling = next;
                    _current = next;
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

        return;
        // haven't worked out the rest yet

        // if (!isExpression()) {
        //     throw std::runtime_error("Missing expression");
        // }

        // TokenNode *rParen = new TokenNode(*_nIt++);
        // if (rParen->type != TokenType::R_PAREN) {
        //     throw std::runtime_error("Missing R-Paren");
        // }
        // _current->sibling = rParen;
        // _current = rParen;

        // _nIt++;
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

        // FOR TESTING, just to get single number
        _current->sibling = unknown;
        _current = unknown;
        return true;

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
    try {
        TokenNode *holderNode = _current;
        auto holderIt = _nIt;

        // check if single operand,
        // if true AND next token isnt relational op - switch to boolExp case checks
        isNumericalExpression();
        TokenNode *next = new TokenNode(*_nIt++);

        if (!_operandFlag || (_operandFlag && isRelationalOperator(next->type))) {
            if (!isRelationalOperator(next->type)) {  // redundent check maybe, play around with fixing the if conditions
                throw std::runtime_error("Expected relational operator");
            }
            _current->sibling = next;
            _current = next;

            if (!isNumericalExpression()) {
                throw std::runtime_error("Expected numerical expression");
            }
            return true;
        } else {
            // clear tree of num, fix this mess
            delete _current;
            _current = holderNode;
            _nIt = holderIt;
            TokenNode *first = new TokenNode(*_nIt++);

            switch (first->type) {
                case TokenType::FALSE:
                case TokenType::TRUE: {
                    // boolean TRUE or FALSE
                    _current->sibling = first;
                    _current = first;
                    return true;
                }
                case TokenType::IDENTIFIER: {
                    _current->sibling = first;
                    _current = first;
                    TokenNode *second = new TokenNode(*_nIt++);

                    // identifier & boolean op & boolean expression (recursion)
                    if (isBooleanOperator(second->type)) {
                        _current->sibling = second;
                        _current = second;
                        if (!isBooleanExpression()) {
                            throw std::runtime_error("Expected boolean expression");
                        }
                        return true;
                    }

                    // identifier
                    else {
                        _nIt--;  // un-gets second
                        delete second;
                        return true;
                    }
                }
                case TokenType::L_PAREN: {
                    // L-Paren & identifier & boolean op & boolean expression (recursion) & R-Paren

                    // add L-Paren to tree
                    _current->sibling = first;
                    _current = first;

                    TokenNode *next = new TokenNode(*_nIt++);
                    if (next->type != TokenType::IDENTIFIER) {
                        throw std::runtime_error("Expected identifier");
                    }

                    // add identifier to tree
                    _current->sibling = next;
                    _current = next;

                    TokenNode *boolOp = new TokenNode(*_nIt++);
                    if (!isBooleanOperator(boolOp->type)) {
                        throw std::runtime_error("Expected boolean operator");
                    }

                    // add boolean operator to tree
                    _current->sibling = boolOp;
                    _current = boolOp;

                    // handles inner boolean expression itself
                    if (!isBooleanExpression()) {
                        throw std::runtime_error("Expected boolean expression");
                    }

                    TokenNode *next = new TokenNode(*_nIt++);
                    if (next->type != TokenType::R_PAREN) {
                        throw std::runtime_error("Expected R-Paren");
                    }

                    // add R-Paren to tree
                    _current->sibling = next;
                    _current = next;

                    return true;
                }
                default: {
                    // numerical expression & boolean relational expression & numerical expression
                    // not handled here anymore, but whats the default case?
                    // error? or return false maybe
                }
            }
            return false;
        }
    } catch (const std::exception &ex) {
        std::cout << "malformed boolean expression: "
                  << ex.what() << std::endl;
        exit(1);
    }
}

bool CSTree::isNumericalExpression() {
    // make flag relevant to deepest call unless set at top level
    _operandFlag = false;

    TokenNode *first = new TokenNode(*_nIt++);
    if (isOperand(first)) {
        // add operand to tree
        _current->sibling = first;
        _current = first;

        // get and check next
        TokenNode *next = new TokenNode(*_nIt++);

        if (isNumericOperator(next->type)) {
            // add operator to tree
            _current->sibling = next;
            _current = next;

            next = new TokenNode(*_nIt++);

            if (next->type == TokenType::L_PAREN) {
                // operand + operator + L-Paren + numExp + R-Paren

                // add L-Paren to tree
                _current->sibling = next;
                _current = next;
                if (!isNumericalExpression()) {
                    throw std::runtime_error("Expected numerical expression");
                }

                next = new TokenNode(*_nIt++);
                if (next->type != TokenType::R_PAREN) {
                    throw std::runtime_error("Missing R-Paren");
                }

                // add R-Paren to tree
                _current->sibling = next;
                _current = next;
                return true;

            } else {
                // operand + operator + numExp
                // unget last token, to be valid this must be a new NumExp
                _nIt--;
                delete next;

                if (!isNumericalExpression()) {
                    throw std::runtime_error("Expected numerical expression");
                }
                _operandFlag = false;
                return true;
            }
        } else {
            // operand
            _nIt--;
            delete next;
            _operandFlag = true;
            return true;
        }
    } else if (first->type == TokenType::L_PAREN) {
        // L-Paren + operand + R-Paren
        // L-Paren + operand + operator + numExp + R-Paren + operand + operator + numExp
        // L-Paren + operand + operator + numExp + R-Paren

        // all this can be basically reduced to ( numExp ) or ( numExp ) operator numExp

        _current->sibling = first;
        _current = first;

        if (!isNumericalExpression()) {
            throw std::runtime_error("Expected numerical expression");
        }

        TokenNode *next = new TokenNode(*_nIt++);
        if (next->type != TokenType::R_PAREN) {
            throw std::runtime_error("Missing R-Paren");
        }
        _current->sibling = next;
        _current = next;
        return true;
    }

    // for testing
    throw std::runtime_error("not numExp");
    return true;
}

bool CSTree::isExpression() {
    // valid if boolean or numerical expression
    // account for recursion
    return (isBooleanExpression() || isNumericalExpression());
}

bool CSTree::isOperand(TokenNode *token) {
    return token->type == TokenType::IDENTIFIER || token->type == TokenType::INTEGER ||
           token->type == TokenType::FUNCTION || token->type == TokenType::CHAR_LITERAL ||
           (token->type == TokenType::STRING && token->lexeme.length() == 1) || isNumericOperator(token->type);
}