#include "cst.hpp"
#include "token_enum.hpp"
#include "token_node.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <stdexcept>

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
    case TokenType::L_BRACE: {
      _openStack.push(TokenType::L_BRACE);
      std::cout << "Pushed L_BRACE\n";
      addChildAndAdvance(next);
      break;
    }
    case TokenType::R_BRACE: {
      if (_openStack.top() != TokenType::L_BRACE) {
        throw std::runtime_error("[line " + std::to_string(next->lineNumber) +
                                 "] " + "R-Brace has no L-Brace");
      }
      _openStack.pop();
      std::cout << "Popped L_BRACE\n";
      addChildAndAdvance(next);
      break;
    }
    case TokenType::ELSE:
    case TokenType::SEMICOLON: {
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
      case TokenType::L_BRACE:
        _openStack.push(TokenType::L_BRACE);
        std::cout << "Pushed L_BRACE\n";
      case TokenType::ELSE:
      case TokenType::SEMICOLON: {
        addChildAndAdvance(next);
        break;
      }
      case TokenType::L_PAREN: {
        _openStack.push(TokenType::L_PAREN);
        std::cout << "Pushed L_PAREN\n";
        addSiblingAndAdvance(next);
        break;
      }
      case TokenType::R_PAREN: {
        if (_openStack.top() != TokenType::L_PAREN) {
          throw std::runtime_error("R-Paren has no L-Paren");
        }
        _openStack.pop();
        std::cout << "Popped L_PAREN\n";
        addSiblingAndAdvance(next);
        break;
      }
      case TokenType::L_BRACKET: {
        _openStack.push(TokenType::L_BRACKET);
        std::cout << "Pushed L_BRACKET\n";
        addSiblingAndAdvance(next);
        break;
      }
      case TokenType::R_BRACKET: {
        if (_openStack.top() != TokenType::L_BRACKET) {
          throw std::runtime_error("R-Bracket has no L-Bracket");
        }
        _openStack.pop();
        std::cout << "Popped L_BRACKET\n";
        addChildAndAdvance(next);
        break;
      }
      default:
        addSiblingAndAdvance(next);
        break;
      }
    }
  }
}

void CSTree::addSiblingAndAdvance(TokenNode *node) {
  _current->sibling = node;
  _current = node;
}

void CSTree::addChildAndAdvance(TokenNode *node) {
  _current->child = node;
  _current = node;
}

void CSTree::isFor() {
  try {
    TokenNode *lParen = new TokenNode(*_nIt++);

    if (lParen->type != TokenType::L_PAREN) {
      throw std::runtime_error("Missing L-Paren");
    }

    _openStack.push(TokenType::L_PAREN);
    std::cout << "Pushed L_PAREN\n";

    addSiblingAndAdvance(lParen);

    if (!isInitializationExpression()) {
      throw std::runtime_error("Missing initialization expression");
    }

    TokenNode *semiColon = new TokenNode(*_nIt++);
    if (semiColon->type != TokenType::SEMICOLON) {
      throw std::runtime_error("Missing semicolon");
    }

    addSiblingAndAdvance(semiColon);
    if (!isBooleanExpression()) {
      throw std::runtime_error("Missing boolean expression");
    }

    TokenNode *semiColon2 = new TokenNode(*_nIt++);
    if (semiColon2->type != TokenType::SEMICOLON) {
      throw std::runtime_error("Missing semicolon");
    }

    addSiblingAndAdvance(semiColon2);

    if (!isNumericalExpression()) {
      throw std::runtime_error("Missing expression");
    }

    TokenNode *rParen = new TokenNode(*_nIt++);
    if (rParen->type != TokenType::R_PAREN) {
      throw std::runtime_error("Missing R-Paren");
    }
    if (_openStack.top() != TokenType::L_PAREN) {
      throw std::runtime_error("R-Paren has no L-Paren");
    }

    _openStack.pop();
    std::cout << "Popped L_PAREN\n";

    addSiblingAndAdvance(rParen);
  } catch (const std::exception &ex) {
    std::cout << "malformed for loop: " << ex.what() << std::endl;
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

    addSiblingAndAdvance(identifier);

    // followed by assignment operator
    TokenNode *assignmentOp = new TokenNode(*_nIt++);
    if (assignmentOp->type != TokenType::ASSIGNMENT_OPERATOR) {
      throw std::runtime_error("Missing assignment operator");
    }

    ;
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
    return true;

    // else, must be an expression to be valid
    // isExpression(); create
    return true;

  } catch (const std::exception &ex) {
    std::cout << "malformed initialization expression: " << ex.what()
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
    isNumericalExpression();
    TokenNode *next = new TokenNode(*_nIt++);

    if (!_operandFlag || (_operandFlag && isRelationalOperator(next->type))) {
      if (!isRelationalOperator(
              next->type)) { // redundent check maybe, play around with fixing
                             // the if conditions
        throw std::runtime_error("Expected relational operator");
      }

      addSiblingAndAdvance(next);

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
            throw std::runtime_error("Expected boolean expression");
          }
          return true;
        }

        // identifier
        else {
          _nIt--; // un-gets second
          delete second;
          return true;
        }
      }
      case TokenType::L_PAREN: {
        // L-Paren & identifier & boolean op & boolean expression (recursion)
        // & R-Paren
        _openStack.push(TokenType::L_PAREN);
        std::cout << "Pushed L_PAREN\n";

        // add L-Paren to tree

        addSiblingAndAdvance(first);

        TokenNode *next = new TokenNode(*_nIt++);
        if (next->type != TokenType::IDENTIFIER) {
          throw std::runtime_error("Expected identifier");
        }

        // add identifier to tree

        addSiblingAndAdvance(next);

        TokenNode *boolOp = new TokenNode(*_nIt++);
        if (!isBooleanOperator(boolOp->type)) {
          throw std::runtime_error("Expected boolean operator");
        }

        // add boolean operator to tree

        addSiblingAndAdvance(boolOp);

        // handles inner boolean expression itself
        if (!isBooleanExpression()) {
          throw std::runtime_error("Expected boolean expression");
        }

        next = new TokenNode(*_nIt++);
        if (next->type != TokenType::R_PAREN) {
          throw std::runtime_error("Expected R-Paren");
        }
        if (_openStack.top() != TokenType::L_PAREN) {
          throw std::runtime_error("R-Paren has no L-Paren");
        }
        _openStack.pop();
        std::cout << "Popped L_PAREN\n";

        // add R-Paren to tree
        addSiblingAndAdvance(next);

        return true;
      }
      default: {
        // numerical expression & boolean relational expression & numerical
        // expression not handled here anymore, but whats the default case?
        // error? or return false maybe
      }
      }
      return false;
    }
  } catch (const std::exception &ex) {
    std::cout << "malformed boolean expression: " << ex.what() << std::endl;
    exit(1);
  }
}

bool CSTree::isNumericalExpression() {
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
        _openStack.push(TokenType::L_PAREN);
        std::cout << "Pushed L_PAREN\n";

        // add L-Paren to tree

        addSiblingAndAdvance(next);

        if (!isNumericalExpression()) {
          throw std::runtime_error("Expected numerical expression");
        }

        next = new TokenNode(*_nIt++);
        if (next->type != TokenType::R_PAREN) {
          throw std::runtime_error("Missing R-Paren");
        }
        if (_openStack.top() != TokenType::L_PAREN) {
          throw std::runtime_error("R-Paren has no L-Paren");
        }
        _openStack.pop();
        std::cout << "Popped L_PAREN\n";

        // add R-Paren to tree
        addSiblingAndAdvance(next);
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
      switch (next->type) {
      case TokenType::PLUS: {
        // i++
        addSiblingAndAdvance(next);
        TokenNode *plus = new TokenNode(*_nIt++);
        if (plus->type != TokenType::PLUS) {
          throw std::runtime_error("Expected plus operator");
        }

        addSiblingAndAdvance(plus);
        return true;
      }
      case TokenType::MINUS: {
        // i--
        addSiblingAndAdvance(next);
        TokenNode *minus = new TokenNode(*_nIt++);
        if (minus->type != TokenType::MINUS) {
          throw std::runtime_error("Expected minus operator");
        }

        addSiblingAndAdvance(minus);
        return true;
      }

      default: {
        // operand
        _nIt--;
        delete next;
        _operandFlag = true;
        return true;
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

      _openStack.push(TokenType::L_PAREN);
      std::cout << "Pushed L_PAREN\n";

      addSiblingAndAdvance(first);

      if (!isNumericalExpression()) {
        throw std::runtime_error("Expected numerical expression");
      }

      TokenNode *next = new TokenNode(*_nIt++);
      if (next->type != TokenType::R_PAREN) {
        throw std::runtime_error("Missing R-Paren");
      }
      if (_openStack.top() != TokenType::L_PAREN) {
        throw std::runtime_error("R-Paren has no L-Paren");
      }
      _openStack.pop();
      std::cout << "Popped L_PAREN\n";

      addSiblingAndAdvance(next);
      return true;
    }
    case TokenType::PLUS: {
      // ++i
      addSiblingAndAdvance(first);
      TokenNode *plus = new TokenNode(*_nIt++);
      if (plus->type != TokenType::PLUS) {
        throw std::runtime_error("Expected plus operator");
      }

      addSiblingAndAdvance(plus);

      TokenNode *identifier = new TokenNode(*_nIt++);
      if (identifier->getTypeName() != "IDENTIFIER") {
        throw std::runtime_error("Expected identifier");
      }

      addSiblingAndAdvance(identifier);
      return true;
    }
    case TokenType::MINUS: {
      // --i
      addSiblingAndAdvance(first);
      TokenNode *minus = new TokenNode(*_nIt++);
      if (minus->type != TokenType::MINUS) {
        throw std::runtime_error("Expected minus operator");
      }
      addSiblingAndAdvance(minus);

      TokenNode *identifier = new TokenNode(*_nIt++);
      if (identifier->getTypeName() != "IDENTIFIER") {
        throw std::runtime_error("Expected identifier");
      }
      addSiblingAndAdvance(identifier);
      return true;
    }
    default:
      throw std::runtime_error("not numExp");
    }
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
  return token->type == TokenType::IDENTIFIER ||
         token->type == TokenType::INTEGER ||
         token->type == TokenType::FUNCTION ||
         token->type == TokenType::CHAR_LITERAL ||
         (token->type == TokenType::STRING && token->lexeme.length() == 1) ||
         isNumericOperator(token->type);
}
