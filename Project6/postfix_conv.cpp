#include <iostream>
#include <stack>

#include "token_enum.hpp"
#include "token_node.hpp"

/* Numerical Expression Shunting Yard */

// should be switched to inserting nodes
void displayToken(TokenNode* currToken) {
    std::cout << currToken->lexeme << " ";
}

void NumExpPostfixConverter(TokenNode* currToken, TokenNode* endToken) {
    std::stack<TokenNode*> _holdStack;
    bool _finished = false;  // looping flag
    TokenNode* topToken = nullptr;

    // 'display' token meaning add to ASTree
    // function could return a pointer to a chain of AST nodes, handled upon return to caller
    // currently: printing to std::cout

    // movement loop
    // endNode must be a sibling, and have be set before function call
    for (; currToken != endToken; currToken = currToken->sibling) {
        switch (currToken->type) {
            case TokenType::INTEGER:
            case TokenType::IDENTIFIER:
            case TokenType::SINGLE_QUOTE:
            case TokenType::DOUBLE_QUOTE:
            case TokenType::STRING: {
                displayToken(currToken);
                break;
            }
            case TokenType::L_PAREN: {
                _holdStack.push(currToken);
                break;
            }
            case TokenType::R_PAREN: {
                topToken = _holdStack.top();
                while (topToken->type != TokenType::L_PAREN) {
                    displayToken(topToken);
                    _holdStack.pop();
                    if (!_holdStack.empty()) {
                        topToken = _holdStack.top();
                    }
                }
                _holdStack.pop();
                break;
            }
            case TokenType::PLUS:
            case TokenType::MINUS:
            case TokenType::ASTERISK:
            case TokenType::DIVIDE:
            case TokenType::MODULO:
            case TokenType::ASSIGNMENT_OPERATOR: {
                if (_holdStack.empty()) {
                    _holdStack.push(currToken);
                } else {
                    switch (currToken->type) {
                        case TokenType::PLUS:
                        case TokenType::MINUS: {
                            _finished = false;
                            while (!_finished) {
                                if (!_holdStack.empty()) {
                                    topToken = _holdStack.top();
                                    if (isNumericOperator(topToken->type)) {
                                        displayToken(topToken);
                                        _holdStack.pop();
                                    } else {
                                        _holdStack.push(currToken);
                                        _finished = true;
                                    }
                                } else {
                                    _holdStack.push(currToken);
                                    _finished = true;
                                }
                            }
                            break;
                        }
                        case TokenType::ASTERISK:
                        case TokenType::DIVIDE:
                        case TokenType::MODULO: {
                            _finished = false;
                            while (!_finished) {
                                if (!_holdStack.empty()) {
                                    topToken = _holdStack.top();
                                    if ((topToken->type == TokenType::ASTERISK) ||
                                        (topToken->type == TokenType::DIVIDE) ||
                                        (topToken->type == TokenType::MODULO)) {
                                        displayToken(topToken);
                                        _holdStack.pop();
                                    } else {
                                        _holdStack.push(currToken);
                                        _finished = true;
                                    }
                                } else {
                                    _holdStack.push(currToken);
                                    _finished = true;
                                }
                            }
                            break;
                        }
                        case TokenType::ASSIGNMENT_OPERATOR: {
                            _finished = false;
                            while (!_finished) {
                                if (!_holdStack.empty()) {
                                    topToken = _holdStack.top();
                                    if (isNumericOperator(topToken->type)) {
                                        displayToken(topToken);
                                        _holdStack.pop();
                                        topToken = _holdStack.top();

                                    } else {
                                        _holdStack.push(currToken);
                                        _finished = true;
                                    }
                                } else {
                                    _holdStack.push(currToken);
                                    _finished = true;
                                }
                            }
                            break;
                        }
                        default: {
                            // error
                        }
                    }
                }
                break;
            }
            default: {
                // error out?
                // should never reach this point, all cst rows should be correct
            }
        }
    }
    while (!_holdStack.empty()) {
        // display token at top of stack
        // pop stack
        topToken = _holdStack.top();
        displayToken(topToken);
        _holdStack.pop();
    }
    std::cout << std::endl;
}

void BoolExpPostfixConverter(TokenNode* currToken, TokenNode* endToken) {
    std::stack<TokenNode*> _holdStack;
    bool _finished = false;  // looping flag
    TokenNode* topToken = nullptr;

    // 'display' token meaning add to ASTree
    // function could return a pointer to a chain of AST nodes, handled upon return to caller
    // currently: printing to std::cout

    // movement loop
    // endNode must be a sibling, and have be set before function call
    for (; currToken != endToken; currToken = currToken->sibling) {
        switch (currToken->type) {
            case TokenType::INTEGER:
            case TokenType::IDENTIFIER:
            case TokenType::SINGLE_QUOTE:
            case TokenType::DOUBLE_QUOTE:
            case TokenType::STRING:
            case TokenType::L_BRACKET:
            case TokenType::R_BRACKET: {
                displayToken(currToken);
                break;
            }
            case TokenType::L_PAREN: {
                _holdStack.push(currToken);
                break;
            }
            case TokenType::R_PAREN: {
                _finished = false;
                while (!_finished) {
                    topToken = _holdStack.top();
                    if (topToken->type == TokenType::L_PAREN) {
                        _holdStack.pop();
                        _finished = true;
                    } else {
                        displayToken(topToken);
                        _holdStack.pop();
                    }
                }
                break;
            }
            case TokenType::BOOLEAN_NOT:
            case TokenType::BOOLEAN_NOT_EQUAL:
            case TokenType::BOOLEAN_EQUAL:
            case TokenType::BOOLEAN_AND:
            case TokenType::BOOLEAN_OR:
            case TokenType::LT:
            case TokenType::LT_EQUAL:
            case TokenType::GT:
            case TokenType::GT_EQUAL:
            case TokenType::PLUS:
            case TokenType::MINUS:
            case TokenType::ASTERISK:
            case TokenType::DIVIDE:
            case TokenType::MODULO: {
                if (_holdStack.empty() || _holdStack.top()->type == TokenType::L_PAREN) {
                    _holdStack.push(currToken);
                } else {
                    switch (currToken->type) {
                        case TokenType::BOOLEAN_NOT: {
                            _finished = false;
                            while (!_finished) {
                                if (!_holdStack.empty()) {
                                    topToken = _holdStack.top();
                                    if (topToken->type == TokenType::BOOLEAN_NOT) {
                                        displayToken(topToken);
                                        _holdStack.pop();
                                    } else {
                                        _holdStack.push(currToken);
                                        _finished = true;
                                    }
                                } else {
                                    _holdStack.push(currToken);
                                    _finished = true;
                                }
                            }
                            break;
                        }
                        case TokenType::ASTERISK:
                        case TokenType::DIVIDE:
                        case TokenType::MODULO: {
                            _finished = false;
                            while (!_finished) {
                                if (!_holdStack.empty()) {
                                    topToken = _holdStack.top();
                                    if (topToken->type == TokenType::BOOLEAN_NOT ||
                                        topToken->type == TokenType::ASTERISK ||
                                        topToken->type == TokenType::DIVIDE ||
                                        topToken->type == TokenType::MODULO) {
                                        displayToken(topToken);
                                        _holdStack.pop();
                                    } else {
                                        _finished = true;
                                        _holdStack.push(currToken);
                                    }
                                } else {
                                    _finished = true;
                                    _holdStack.push(currToken);
                                }
                            }
                            break;
                        }
                        case TokenType::PLUS:
                        case TokenType::MINUS: {
                            _finished = false;
                            while (!_finished) {
                                if (!_holdStack.empty()) {
                                    topToken = _holdStack.top();
                                    if ((topToken->type == TokenType::BOOLEAN_NOT) || isNumericOperator(topToken->type)) {
                                        displayToken(topToken);
                                        _holdStack.pop();
                                    } else {
                                        _holdStack.push(currToken);
                                        _finished = true;
                                    }
                                } else {
                                    _holdStack.push(currToken);
                                    _finished = true;
                                }
                            }
                            break;
                        }
                        case TokenType::BOOLEAN_NOT_EQUAL:
                        case TokenType::BOOLEAN_EQUAL:
                        case TokenType::LT:
                        case TokenType::LT_EQUAL:
                        case TokenType::GT:
                        case TokenType::GT_EQUAL: {
                            _finished = false;
                            while (!_finished) {
                                if (!_holdStack.empty()) {
                                    topToken = _holdStack.top();
                                    if (isOperator(topToken->type) &&
                                        topToken->type != TokenType::BOOLEAN_AND &&
                                        topToken->type != TokenType::BOOLEAN_OR) {
                                        displayToken(topToken);
                                        _holdStack.pop();
                                    } else {
                                        _holdStack.push(currToken);
                                        _finished = true;
                                    }
                                } else {
                                    _holdStack.push(currToken);
                                    _finished = true;
                                }
                            }
                            break;
                        }
                        case TokenType::BOOLEAN_AND:
                        case TokenType::BOOLEAN_OR: {
                            _finished = false;
                            while (!_finished) {
                                if (!_holdStack.empty()) {
                                    topToken = _holdStack.top();
                                    if (isOperator(topToken->type)) {
                                        displayToken(topToken);
                                        _holdStack.pop();
                                    } else {
                                        _holdStack.push(currToken);
                                        _finished = true;
                                    }
                                } else {
                                    _holdStack.push(currToken);
                                    _finished = true;
                                }
                            }
                            break;
                        }
                        default: {
                            // error
                        }
                    }
                }
                break;
            }
            default: {
                // error
            }
        }
    }

    while (!_holdStack.empty()) {
        // display token at top of stack
        // pop stack
        topToken = _holdStack.top();
        displayToken(topToken);
        _holdStack.pop();
    }
    std::cout << std::endl;
}
