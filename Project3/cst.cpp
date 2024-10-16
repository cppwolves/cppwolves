#include "cst.hpp"

#include <iostream>
#include <stdexcept>

#include "token_enum.hpp"
#include "token_node.hpp"

CSTree::CSTree(std::vector<Token> &tokens) {
    if (tokens.empty()) {
        // return nullptr;
        // deal with, what behavior?
    }

    _head = new TokenNode(tokens.at(0));
    _current = _head;

    _nIt = tokens.begin();

    for (_nIt++; _nIt != tokens.end(); _nIt++) {
        // First, check the next node
        TokenNode *next = new TokenNode(*_nIt);
        switch (next->type) {
            // The next token is a delimiter
            case TokenType::L_BRACE:
            case TokenType::R_BRACE:
            case TokenType::ELSE: {
                addChildAndAdvance(next);
                break;
            }
            default:
                // Default to the current node
                switch (_current->type) {
                    case TokenType::FOR: {
                        isFor();
                        break;
                    }
                    case TokenType::WHILE: {
                        isWhile();
                        break;
                    }
                    case TokenType::FUNCTION: {
                        isFunction();
                        break;
                    }
                    case TokenType::PROCEDURE: {
                        isProcedure();
                        break;
                    }
                    // case TokenType::INT:
                    // case TokenType::CHAR:
                    // case TokenType::BOOL: {
                    //     isDatatypeSpecifier();
                    //     break;
                    // }
                    case TokenType::L_BRACE:
                    case TokenType::R_BRACE:
                    case TokenType::ELSE:
                    case TokenType::SEMICOLON: {
                        addChildAndAdvance(next);
                        break;
                    }
                    default: {
                        addSiblingAndAdvance(next);
                        break;
                    }
                }
        }
    }
}

void CSTree::addSiblingAndAdvance(TokenNode *node) {
    handleOpenCloseDelimiters(node);
    _current->sibling = node;
    _current = node;
}

void CSTree::addChildAndAdvance(TokenNode *node) {
    handleOpenCloseDelimiters(node);
    _current->child = node;
    _current = node;
}

void CSTree::throwTokenError(TokenNode *node, const std::string &message) {
    throw std::runtime_error("[line " + std::to_string(node->lineNumber) +
                             "]: " + message);
}

void CSTree::handleOpenCloseDelimiters(TokenNode *node) {
    switch (node->type) {
        case TokenType::L_BRACE: {
            _openStack.push(TokenType::L_BRACE);
            break;
        }
        case TokenType::L_PAREN: {
            _openStack.push(TokenType::L_PAREN);
            break;
        }
        case TokenType::R_PAREN: {
            if (_openStack.top() != TokenType::L_PAREN) {
                throwTokenError(node, "R-Paren has no L-Paren");
            }
            _openStack.pop();
            break;
        }
        case TokenType::L_BRACKET: {
            _openStack.push(TokenType::L_BRACKET);
            break;
        }
        case TokenType::R_BRACKET: {
            if (_openStack.top() != TokenType::L_BRACKET) {
                throwTokenError(node, "R-Bracket has no L-Bracket");
            }
            _openStack.pop();
            break;
        }
    }
}

void CSTree::isFor() {
    try {
        TokenNode *lParen = new TokenNode(*_nIt++);

        if (lParen->type != TokenType::L_PAREN) {
            throwTokenError(lParen, "Missing L-Paren");
        }

        addSiblingAndAdvance(lParen);

        if (!isInitializationExpression()) {
            throwTokenError(lParen, "Missing initialization expression");
        }

        TokenNode *semiColon = new TokenNode(*_nIt++);
        if (semiColon->type != TokenType::SEMICOLON) {
            throwTokenError(semiColon, "Missing semicolon");
        }

        addSiblingAndAdvance(semiColon);
        if (!isBooleanExpression()) {
            throwTokenError(semiColon, "Missing boolean expression");
        }

        TokenNode *semiColon2 = new TokenNode(*_nIt++);
        if (semiColon2->type != TokenType::SEMICOLON) {
            throwTokenError(semiColon2, "Missing semicolon");
        }

        addSiblingAndAdvance(semiColon2);

        if (!isNumericalExpression()) {
            throwTokenError(semiColon2, "Missing expression");
        }

        TokenNode *rParen = new TokenNode(*_nIt);
        if (rParen->type != TokenType::R_PAREN) {
            throwTokenError(rParen, "Missing R-Paren");
        }
        if (_openStack.top() != TokenType::L_PAREN) {
            throwTokenError(rParen, "R-Paren has no L-Paren");
        }

        addSiblingAndAdvance(rParen);
    } catch (const std::exception &ex) {
        std::cerr << "malformed for loop: " << ex.what() << std::endl;
        exit(1);
    }
}

void CSTree::isWhile() {
    try {
        TokenNode *lParen = new TokenNode(*_nIt++);

        if (lParen->type != TokenType::L_PAREN) {
            throwTokenError(lParen, "Missing L-Paren");
        }

        addSiblingAndAdvance(lParen);

        if (!isBooleanExpression()) {
            throwTokenError(lParen, "Missing boolean expression");
        }

        TokenNode *rParen = new TokenNode(*_nIt);
        if (rParen->type != TokenType::R_PAREN) {
            throwTokenError(rParen, "Missing R-Paren");
        }
        if (_openStack.top() != TokenType::L_PAREN) {
            throwTokenError(rParen, "R-Paren has no L-Paren");
        }

        addSiblingAndAdvance(rParen);
    } catch (const std::exception &ex) {
        std::cerr << "malformed for loop: " << ex.what() << std::endl;
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
            throwTokenError(identifier, "Missing indentifier");
        }

        addSiblingAndAdvance(identifier);

        // followed by assignment operator
        TokenNode *assignmentOp = new TokenNode(*_nIt++);
        if (assignmentOp->type != TokenType::ASSIGNMENT_OPERATOR) {
            throwTokenError(assignmentOp, "Missing assignment operator");
        }

        addSiblingAndAdvance(assignmentOp);

        // can be string OR expression
        TokenNode *unknown = new TokenNode(*_nIt++);
        if (unknown->type == TokenType::STRING) {
            _current->sibling = unknown;
            _current = unknown;
            return true;
        }

        // FOR TESTING, just to get single number
        addSiblingAndAdvance(unknown);

        // else, must be an expression to be valid
        // isExpression(); create
        return true;

    } catch (const std::exception &ex) {
        std::cerr << "malformed initialization expression: " << ex.what()
                  << std::endl;
        exit(1);
    }
}

bool CSTree::isBooleanExpression() {
    try {
        TokenNode *holderNode = _current;
        auto holderIt = _nIt;

        // check if single operand,
        // if true AND next token isnt relational op - switch to boolExp case
        // checks

        if (isNumericalExpression() ||
            (!_operandFlag || (_operandFlag && isRelationalOperator(_nIt->type)))) {
            TokenNode *next = new TokenNode(*_nIt++);

            addSiblingAndAdvance(next);
            if (next->type == TokenType::TRUE || next->type == TokenType::FALSE) {
                return true;
            }

            if (!isNumericalExpression() && !isBooleanExpression()) {
                throwTokenError(next, "Expected numerical expression");
            }
            if (isBooleanOperator(_nIt->type)) {
                addSiblingAndAdvance(new TokenNode(*_nIt++));
                return isBooleanExpression();
            }
            return true;
        } else {
            // clear tree of num, fix this mess
            revertState(holderNode);
            TokenNode *first = new TokenNode(*_nIt++);

            switch (first->type) {
                case TokenType::FALSE:
                case TokenType::TRUE: {
                    // boolean TRUE or FALSE

                    addSiblingAndAdvance(first);
                    return true;
                }
                case TokenType::IDENTIFIER: {
                    addSiblingAndAdvance(first);
                    TokenNode *second = new TokenNode(*_nIt++);

                    // identifier & boolean op & boolean expression (recursion)
                    if (isBooleanOperator(second->type)) {
                        addSiblingAndAdvance(second);
                        if (!isBooleanExpression()) {
                            throwTokenError(second, "Expected boolean expression");
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
                    // L-Paren & identifier & boolean op & boolean expression (recursion)
                    // & R-Paren

                    // add L-Paren to tree

                    addSiblingAndAdvance(first);

                    TokenNode *next = new TokenNode(*_nIt++);
                    if (next->type != TokenType::IDENTIFIER) {
                        throwTokenError(next, "Expected identifier");
                    }

                    // add identifier to tree

                    addSiblingAndAdvance(next);

                    TokenNode *boolOp = new TokenNode(*_nIt++);
                    if (!isBooleanOperator(boolOp->type)) {
                        throwTokenError(boolOp, "Expected boolean operator");
                    }

                    // add boolean operator to tree

                    addSiblingAndAdvance(boolOp);

                    // handles inner boolean expression itself
                    if (!isBooleanExpression()) {
                        throwTokenError(boolOp, "Expected boolean expression");
                    }

                    next = new TokenNode(*_nIt++);
                    if (next->type != TokenType::R_PAREN) {
                        throwTokenError(next, "Expected R-Paren");
                    }

                    // add R-Paren to tree
                    addSiblingAndAdvance(next);

                    return true;
                }
                default: {
                    // numerical expression & boolean relational expression & numerical
                    // expression not handled here anymore, but whats the default case?
                    // error? or return false maybe
                    revertState(holderNode);
                    return false;
                }
            }
            revertState(holderNode);
            return false;
        }
    } catch (const std::exception &ex) {
        std::cerr << "malformed boolean expression: " << ex.what() << std::endl;
        exit(1);
    }
}

bool CSTree::isNumericalExpression() {
    TokenNode *holderNode = _current;
    auto holderIt = _nIt;

    // make flag relevant to deepest call unless set at top level
    _operandFlag = false;

    TokenNode *first = new TokenNode(*_nIt++);
    if (isOperand(first)) {
        // add operand to tree

        addSiblingAndAdvance(first);

        // get and check next
        TokenNode *next = new TokenNode(*_nIt++);

        if (isNumericOperator(next->type)) {
            // add operator to tree

            addSiblingAndAdvance(next);

            next = new TokenNode(*_nIt++);

            if (next->type == TokenType::L_PAREN) {
                // operand + operator + L-Paren + numExp + R-Paren

                // add L-Paren to tree

                addSiblingAndAdvance(next);

                if (!isNumericalExpression()) {
                    throwTokenError(next, "Expected numerical expression");
                }

                next = new TokenNode(*_nIt++);
                if (next->type != TokenType::R_PAREN) {
                    throwTokenError(next, "Missing R-Paren");
                }

                // add R-Paren to tree
                addSiblingAndAdvance(next);
                return true;

            } else {
                // operand + operator + numExp
                // unget last token, to be valid this must be a new NumExp
                _nIt--;
                delete next;

                if (!isNumericalExpression()) {
                    throwTokenError(first, "Expected numerical expression");
                }
                _operandFlag = false;
                return true;
            }
        } else {
            switch (next->type) {
                case TokenType::PLUS: {
                    // i++
                    addSiblingAndAdvance(next);
                    TokenNode *plus = new TokenNode(*_nIt++);
                    if (plus->type != TokenType::PLUS) {
                        throwTokenError(plus, "Expected plus operator");
                    }

                    addSiblingAndAdvance(plus);
                    return true;
                }
                case TokenType::MINUS: {
                    // i--
                    addSiblingAndAdvance(next);
                    TokenNode *minus = new TokenNode(*_nIt++);
                    if (minus->type != TokenType::MINUS) {
                        throwTokenError(minus, "Expected minus operator");
                    }

                    addSiblingAndAdvance(minus);
                    return true;
                }

                default: {
                    // operand
                    _nIt--;
                    TokenType type = next->type;

                    delete next;
                    _operandFlag = true;

                    return !isRelationalOperator(type);
                }
            }
        }
    } else {
        switch (first->type) {
            case TokenType::L_PAREN: {
                // L-Paren + operand + R-Paren
                // L-Paren + operand + operator + numExp + R-Paren + operand +
                // operator
                // + numExp L-Paren + operand + operator + numExp + R-Paren

                // all this can be basically reduced to ( numExp ) or ( numExp )
                // operator numExp

                addSiblingAndAdvance(first);

                if (!isNumericalExpression()) {
                    if (isRelationalOperator(_nIt->type)) {
                        return false;
                    }
                    throwTokenError(first, "Expected numerical expression");
                }

                TokenNode *next = new TokenNode(*_nIt++);
                if (next->type != TokenType::R_PAREN) {
                    throwTokenError(next, "Missing R-Paren");
                }

                addSiblingAndAdvance(next);
                return true;
            }
            case TokenType::PLUS: {
                // ++i
                addSiblingAndAdvance(first);
                TokenNode *plus = new TokenNode(*_nIt++);
                if (plus->type != TokenType::PLUS) {
                    throwTokenError(plus, "Expected plus operator");
                }

                addSiblingAndAdvance(plus);

                TokenNode *identifier = new TokenNode(*_nIt++);
                if (identifier->getTypeName() != "IDENTIFIER") {
                    throwTokenError(identifier, "Expected identifier");
                }

                addSiblingAndAdvance(identifier);
                return true;
            }
            case TokenType::MINUS: {
                // --i
                addSiblingAndAdvance(first);
                TokenNode *minus = new TokenNode(*_nIt++);
                if (minus->type != TokenType::MINUS) {
                    throwTokenError(minus, "Expected minus operator");
                }
                addSiblingAndAdvance(minus);

                TokenNode *identifier = new TokenNode(*_nIt++);
                if (identifier->getTypeName() != "IDENTIFIER") {
                    throwTokenError(identifier, "Expected identifier");
                }
                addSiblingAndAdvance(identifier);
                return true;
            }
            default:
                revertState(holderNode);
                return false;
                // throwTokenError(first, "not numExp");
        }
    }
    // for testing
    throw std::runtime_error("not numExp");
    return true;
}

void CSTree::revertState(TokenNode *node) {
    _current = node;
    _nIt--;

    node = node->sibling ? node->sibling : node->child;
    while (node) {
        TokenNode *tempNode = node;
        if (tempNode->sibling) {
            node = tempNode->sibling;
        } else {
            node = tempNode->child;
        }
        delete tempNode;
        _nIt--;
    }
}

// i think this function has a serious flaw with how the exceptions are handled
// does it ever check isNumericalExp if isBool fails, or does it throw ex first
bool CSTree::isExpression() {
    // valid if boolean or numerical expression
    // account for recursion
    return (isBooleanExpression() || isNumericalExpression());
}

bool CSTree::isOperand(TokenNode *token) {
    return token->type == TokenType::IDENTIFIER ||
           token->type == TokenType::INTEGER ||
           token->type == TokenType::FUNCTION ||
           token->type == TokenType::CHAR_LITERAL ||
           (token->type == TokenType::STRING && token->lexeme.length() == 1) ||
           isNumericOperator(token->type);
}

void CSTree::isFunction() {
    try {
        TokenNode *next = new TokenNode(*_nIt++);

        // identifer for datatype

        if (next->getTypeName() != "IDENTIFIER") {
            throwTokenError(next, "Expecting identifier");
        }

        addSiblingAndAdvance(next);

        next = new TokenNode(*_nIt++);

        // identifer for function name
        if (next->getTypeName() != "IDENTIFIER") {
            throwTokenError(next, "Expecting identifier");
        } else if (isKeyword(next->type)) {
            throwTokenError(next, "Keyword cannot be a function name");
        }

        addSiblingAndAdvance(next);

        next = new TokenNode(*_nIt++);

        // parenthesis
        if (next->type != TokenType::L_PAREN) {
            throwTokenError(next, "Expecting L-Paren");
        }
        addSiblingAndAdvance(next);
        _openStack.push(TokenType::L_PAREN);

        // handle inside of ()
        if (!isParameterList()) {
            throwTokenError(next, "Expecting parameter list");
        }

        next = new TokenNode(*_nIt);
        if (next->type != TokenType::R_PAREN) {
            throwTokenError(next, "Missing R-Paren");
        }
        if (_openStack.top() != TokenType::L_PAREN) {
            throwTokenError(next, "R-Paren has no L-Paren");
        }

        addSiblingAndAdvance(next);

    } catch (const std::exception &ex) {
        std::cerr << "malformed function: " << ex.what() << std::endl;
        exit(1);
    }
}

bool CSTree::isParameterList() {
    try {
        // datatype identifier
        TokenNode *next = new TokenNode(*_nIt++);
        if (next->getTypeName() != "IDENTIFIER") {
            throwTokenError(next, "Expected datatype");
        }

        addSiblingAndAdvance(next);

        // variable name
        next = new TokenNode(*_nIt++);
        if (next->getTypeName() != "IDENTIFIER") {
            throwTokenError(next, "Expected identifier");
        } else if (isKeyword(next->type)) {
            throwTokenError(next, "Keyword cannot be a variable name");
        }
        addSiblingAndAdvance(next);

        next = new TokenNode(*_nIt++);

        // array
        if (next->type == TokenType::L_BRACKET) {
            addSiblingAndAdvance(next);

            // array index
            next = new TokenNode(*_nIt++);

            // doesnt currently account for variables be used to access arrays
            if (next->type != TokenType::INTEGER || std::stoi(next->lexeme) < 0) {
                throwTokenError(next, "Array indices must be a positive integer");
            }
            addSiblingAndAdvance(next);

            // right bracket
            next = new TokenNode(*_nIt++);
            if (next->type != TokenType::R_BRACKET) {
                // doesnt currently account for numExp or anything other than whole,
                // positive int
                throwTokenError(next, "Expected R-Bracket");
            }
            addSiblingAndAdvance(next);

            // get next token to allow comma check
            next = new TokenNode(*_nIt++);
        }
        if (next->type == TokenType::COMMA) {
            addSiblingAndAdvance(next);

            _chainCheck = true;
            isParameterList();
            _chainCheck = false;
        } else {
            // ptr? reference? can we just ignore it all for now
            _nIt--;  // unget
            delete next;
        }
        return true;

    } catch (const std::exception &ex) {
        if (_chainCheck || _paramCheck) {
            return false;
        } else {
            std::cerr << "malformed parameter list: " << ex.what() << std::endl;
            exit(1);
        }
    }
}

void CSTree::isDatatypeSpecifier() {
    try {
        TokenNode *next = new TokenNode(*_nIt++);

        if (next->getTypeName() != "IDENTIFIER") {
            throwTokenError(next, "Expected identifier");
        } else if (isKeyword(next->type)) {
            throwTokenError(next, "Keyword cannot be a variable name");
        }
        addSiblingAndAdvance(next);
        if (next->type == TokenType::SEMICOLON) {
            return;
        }
        while (next->type != TokenType::SEMICOLON) {
            next = new TokenNode(*_nIt++);
            addSiblingAndAdvance(next);
        }

        return;
        // early return, figure out other stuff
        // either initialization OR assignment OR statement OR ...

    } catch (const std::exception &ex) {
        std::cerr << "malformed datatype specifier: " << ex.what() << std::endl;
        exit(1);
    }
}

void CSTree::isProcedure() {
    // identifer
    try {
        TokenNode *next = new TokenNode(*_nIt++);

        // main procedure, treat uniquely
        if (next->type == TokenType::MAIN) {
            addSiblingAndAdvance(next);
            isMain();
            return;
        }

        // else, named procedure
        if (next->getTypeName() != "IDENTIFIER") {
            throwTokenError(next, "Expected identifier");
        } else if (isKeyword(next->type)) {
            throwTokenError(next, "Keyword cannot be used as procedure name");
        }

        addSiblingAndAdvance(next);

        next = new TokenNode(*_nIt++);

        // L-Paren
        if (next->type != TokenType::L_PAREN) {
            throwTokenError(next, "Missing L-Paren");
        }
        _openStack.push(TokenType::L_PAREN);
        addSiblingAndAdvance(next);
        next = new TokenNode(*_nIt++);

        // handle inside of (), can be void or paramList
        if (next->type == TokenType::VOID) {
            addSiblingAndAdvance(next);
        } else {
            _nIt--;  // delete next? lost mem
            _paramCheck = true;
            if (!isParameterList()) {
                throwTokenError(next, "Expecting parameter list or void");
            }
            _paramCheck = false;
        }

        next = new TokenNode(*_nIt);

        // R-Paren
        if (next->type != TokenType::R_PAREN) {
            throwTokenError(next, "Missing R-Paren");
        }
        if (_openStack.top() != TokenType::L_PAREN) {
            throwTokenError(next, "R-Paren has no L-Paren");
        }

        addSiblingAndAdvance(next);

    } catch (const std::exception &ex) {
        std::cerr << "malformed procedure: " << ex.what() << std::endl;
        exit(1);
    }
}

void CSTree::isMain() {
    try {
        // L-Paren
        TokenNode *next = new TokenNode(*_nIt++);

        if (next->type != TokenType::L_PAREN) {
            throwTokenError(next, "Missing L-Paren");
        }
        _openStack.push(TokenType::L_PAREN);
        addSiblingAndAdvance(next);

        next = new TokenNode(*_nIt++);

        // handle inside of (), must be void
        if (next->type != TokenType::VOID) {
            throwTokenError(next, "Main procedure must have no parameters");
        }
        addSiblingAndAdvance(next);

        // R-Paren
        next = new TokenNode(*_nIt);
        if (next->type != TokenType::R_PAREN) {
            throwTokenError(next, "Missing R-Paren");
        }
        if (_openStack.top() != TokenType::L_PAREN) {
            throwTokenError(next, "R-Paren has no L-Paren");
        }

        addSiblingAndAdvance(next);
    } catch (const std::exception &ex) {
        std::cerr << "malformed main procedure: " << ex.what() << std::endl;
        exit(1);
    }
}