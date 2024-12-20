#include "ast.hpp"
#include "symbol_table_list_node.hpp"
#include "token_enum.hpp"

#include <iostream>
#include <ostream>

ASTree::ASTree(CSTree *cTree, SymbolTable *symTable)
    : cTree(cTree), symTable(symTable), _current(nullptr) {
  // ast node: _current
  // cst node: _currCNode
  _currCNode = cTree->head();
  pushNextScope();

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
      if (_currCNode->type == TokenType::FUNCTION ||
          _currCNode->type == TokenType::PROCEDURE) {
        pushNextScope();
      }
      do {
        addNext(parseDeclaration());
      } while (_currCNode->type == TokenType::COMMA);
      break;
    }
    case TokenType::IF:
    case TokenType::WHILE: {
      pushNextScope();
      // handle postfix bool exp
      addNext(parseBooleanExp());
      break;
    }
    case TokenType::ELSE: {
      auto node = new ASTListNode(ASTNodeType::ELSE);
      node->token = _currCNode;
      pushNextScope();
      addNext(node);
      advance();
      break;
    }
    case TokenType::FOR: {
      // handle FOR (break into 3)
      pushNextScope();
      addNext(parseFor());
      advance();
      break;
    }
    case TokenType::PRINTF: {
      addNext(parsePrintf());
      break;
    }
    case TokenType::RETURN: {
      // handle return
      addNext(parseReturn());
      break;
    }
    case TokenType::IDENTIFIER: {
      // can be: assignment OR function call
      if (_currCNode->sibling->type == TokenType::L_PAREN) {
        addNext(parseCall());
      } else {
        addNext(parseAssignment());
        advance();
      }
      // advance();
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
      popScope();
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

void ASTree::pushNextScope() { _scopeStack.push_back(_nextScope++); }

void ASTree::popScope() { _scopeStack.pop_back(); }

// adding child directly down currently, should be added to the sib of current
void ASTree::addNext(ASTListNode *node) {
  if (!_head) {
    _head = node;
    _current = _head;
  } else {
    _current->child = node;
    _current = _current->child;
  }
  while (_current->sibling) {
    _current = _current->sibling;
  }
}

// clean up after, leave in blocks for needed debugging
void ASTree::advance() {
  // std::cout << "a " << _currCNode->lexeme << std::endl;
  if (_currCNode->sibling) {
    _currCNode = _currCNode->sibling;
  } else if (_currCNode->child) {
    _currCNode = _currCNode->child;
  } else {
    _currCNode = nullptr;
  }
  return;
}

ASTListNode *ASTree::parseDeclaration() {
  ASTListNode *node = new ASTListNode(ASTNodeType::DECLARATION);
  bool _function = _currCNode->type == TokenType::FUNCTION;

  // reach identifer, should only be max twice (function + type + [name])
  while (_currCNode->type != TokenType::IDENTIFIER &&
         _currCNode->type != TokenType::MAIN) {
    advance();
  }

  node->symbol = getNodeSymbol(_currCNode);
  node->token = _currCNode;

  // find either end of current dec, or end of line
  // or, if function, loop through parameter list

  while (_function && !isDelimiter(_currCNode->type)) {
    advance();
  }
  // std::cout << _currCNode->lexeme << std::end;
  while (_currCNode->type != TokenType::COMMA &&
         !isDelimiter(_currCNode->type)) {
    advance();
  }

  // return DECLARATION
  return node;
}

// IF or WHILE
ASTListNode *ASTree::parseBooleanExp() {
  ASTListNode *node = new ASTListNode(tokenTypeToASType(_currCNode->type));
  node->token = _currCNode;
  advance();

  // convert exp
  ASTListNode *sibList = nullptr;
  _currCNode = boolPostfixConverter(_currCNode, sibList);
  node->sibling = sibList;

  return node;
}

ASTListNode *ASTree::parseFor() {
  ASTListNode *node = new ASTListNode(ASTNodeType::FOR1);
  node->token = _currCNode;
  _currCNode = _currCNode->sibling->sibling; // skip paren

  // convert num exp
  ASTListNode *sibList = nullptr;
  _currCNode = numPostfixConverter(_currCNode, sibList);
  node->sibling = sibList;

  // add FOR1
  addNext(node);

  node = new ASTListNode(ASTNodeType::FOR2);
  _currCNode = _currCNode->sibling; // skip semicolon
  sibList = nullptr;
  _currCNode = boolPostfixConverter(_currCNode, sibList);
  node->sibling = sibList;

  // add FOR2
  addNext(node);

  node = new ASTListNode(ASTNodeType::FOR3);
  _currCNode = _currCNode->sibling; // skip semicolon

  sibList = nullptr;
  _currCNode = numPostfixConverter(_currCNode, sibList);
  _currCNode = _currCNode->child;
  node->sibling = sibList;

  // return FOR3
  return node;
}

ASTListNode *ASTree::parseCall() {
  ASTListNode *node = new ASTListNode(ASTNodeType::CALL);
  node->token = _currCNode;
  _currCNode = _currCNode->sibling->sibling; // skip paren

  ASTListNode *sibList = nullptr;
  ASTListNode *lastSibling = nullptr;

  // need to actually parse parameters
  // could be: identifiers, nums, chars, strings, functions, arrays, etc.
  while (_currCNode->type != TokenType::R_PAREN) {
    if (_currCNode->type == TokenType::COMMA) {
      advance();
      continue;
    }
    ASTListNode *param = new ASTListNode(ASTNodeType::SIBLING);
    param->token = _currCNode;
    if (param->token->type == TokenType::IDENTIFIER) {
      param->symbol = getNodeSymbol(param->token);
    }
    if (sibList == nullptr) {
      sibList = param;
    } else {
      lastSibling->sibling = param;
    }

    lastSibling = param;
    advance();
  }

  // skip );
  _currCNode = _currCNode->sibling->child;
  node->sibling = sibList;

  // return CALL
  return node;
}

ASTListNode *ASTree::parseAssignment() {
  ASTListNode *node = new ASTListNode(ASTNodeType::ASSIGNMENT);
  node->token = _currCNode;

  // convert exp
  ASTListNode *sibList = nullptr;
  _currCNode = numPostfixConverter(_currCNode, sibList);
  node->sibling = sibList;

  // return ASSIGNMENT
  return node;
}

ASTListNode *ASTree::parseReturn() {
  ASTListNode *node = new ASTListNode(ASTNodeType::RETURN);
  node->token = _currCNode;

  // convert exp, doesnt work if assignment
  ASTListNode *sibList = nullptr;
  _currCNode = boolPostfixConverter(_currCNode, sibList);
  node->sibling = sibList;

  // return RETURN
  return node;
}

ASTListNode *ASTree::parsePrintf() {
  ASTListNode *node = new ASTListNode(ASTNodeType::PRINTF);
  node->token = _currCNode;

  // skip ("
  _currCNode = _currCNode->sibling->sibling->sibling;

  // get string
  ASTListNode *str = new ASTListNode(ASTNodeType::SIBLING);
  str->token = _currCNode;
  node->sibling = str;

  // skip "
  _currCNode = _currCNode->sibling->sibling;

  ASTListNode *lastSibling = str;

  // need to actually parse parameters
  // could be: identifiers, nums, chars, strings, functions, arrays, etc.
  while (_currCNode->type != TokenType::R_PAREN) {
    if (_currCNode->type == TokenType::COMMA) {
      advance();
      continue;
    }
    ASTListNode *param = new ASTListNode(ASTNodeType::SIBLING);
    param->token = _currCNode;
    lastSibling->sibling = param;
    lastSibling = param;
    advance();
  }

  // skip );
  _currCNode = _currCNode->sibling->child;

  // return PRINTF
  return node;
}

// for block delimiters, not handling multiline strings
bool ASTree::isDelimiter(TokenType type) {
  return (type == TokenType::L_BRACE || type == TokenType::R_BRACE ||
          type == TokenType::SEMICOLON);
}

void ASTree::displayToken(TokenNode *currToken) {
  std::cout << currToken->lexeme << " ";
}

void ASTree::displayToken(TokenNode *currToken, ASTListNode *&_tokenStr,
                          ASTListNode *&_tail) {
  // std::cout << "t: " << currToken->lexeme << "\n";

  if (!_tail) {
    _tokenStr = new ASTListNode(ASTNodeType::SIBLING);
    _tail = _tokenStr;
  } else {
    _tail->sibling = new ASTListNode(ASTNodeType::SIBLING);
    _tail = _tail->sibling;
  }
  _tail->token = currToken;

  if (currToken->type == TokenType::IDENTIFIER) {
    _tail->symbol = getNodeSymbol(currToken);
  }
}

SymbolTableListNode *ASTree::getNodeSymbol(TokenNode *tokenNode) {
  for (int i = _scopeStack.size() - 1; i >= 0; i--) {
    auto *sym = symTable->find(tokenNode->lexeme, _scopeStack.at(i));
    if (sym) {
      return sym;
    }
  }
  return nullptr;
}

TokenNode *ASTree::numPostfixConverter(TokenNode *&currToken,
                                       ASTListNode *&_tokenStr) {
  std::stack<TokenNode *> _holdStack;
  bool _finished = false; // looping flag
  bool _function = false;
  TokenNode *topToken = nullptr;
  ASTListNode *_tail = nullptr;
  TokenNode *_retPosition = nullptr;

  // 'display' token meaning add to ASTree
  // function could return a pointer to a chain of AST nodes, handled upon
  // return to caller currently: printing to std::cout

  // movement loop
  // endNode must be a sibling, and have be set before function call
  for (; (currToken && currToken->type != TokenType::SEMICOLON);
       currToken = currToken->sibling) {
    if (!currToken->sibling) {
      _retPosition = currToken; // for handling returns once row is done
      if (currToken->type == TokenType::R_PAREN) { // end of for exp
        break;
      }
    }
    switch (currToken->type) {
    case TokenType::INTEGER:
    case TokenType::TRUE:
    case TokenType::FALSE:
    case TokenType::SINGLE_QUOTE:
    case TokenType::DOUBLE_QUOTE:
    case TokenType::CHAR_LITERAL:
    case TokenType::STRING:
    case TokenType::L_BRACKET:
    case TokenType::R_BRACKET: {
      displayToken(currToken, _tokenStr, _tail);
      break;
    }
    case TokenType::IDENTIFIER: {
      auto *sym = getNodeSymbol(currToken);
      if (sym && sym->identifierType == TokenType::FUNCTION) {
        _function = true;
      }
      displayToken(currToken, _tokenStr, _tail);
      break;
    }
    case TokenType::L_PAREN: {
      (_function) ? displayToken(currToken, _tokenStr, _tail)
                  : _holdStack.push(currToken);
      break;
    }
    case TokenType::R_PAREN: {
      if (_function) {
        displayToken(currToken, _tokenStr, _tail);
        _function = false;
      } else {
        topToken = _holdStack.top();
        while (topToken->type != TokenType::L_PAREN) {
          displayToken(topToken, _tokenStr, _tail);
          _holdStack.pop();
          if (!_holdStack.empty()) {
            topToken = _holdStack.top();
          }
        }
        _holdStack.pop();
      }
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
                displayToken(topToken, _tokenStr, _tail);
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
                displayToken(topToken, _tokenStr, _tail);
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
                displayToken(topToken, _tokenStr, _tail);
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
    displayToken(topToken, _tokenStr, _tail);
    _holdStack.pop();
  }
  // std::cout << std::endl;

  return (_retPosition ? _retPosition : currToken);
}

TokenNode *ASTree::boolPostfixConverter(TokenNode *&currToken,
                                        ASTListNode *&_tokenStr) {
  std::stack<TokenNode *> _holdStack;
  bool _finished = false; // looping flag
  bool _function = false;
  TokenNode *topToken = nullptr;
  ASTListNode *_tail = nullptr;
  TokenNode *_retPosition = nullptr;

  // 'display' token meaning add to ASTree
  // function could return a pointer to a chain of AST nodes, handled upon
  // return to caller currently: printing to std::cout

  // movement loop
  // ends upon encountering SEMICOLON or L_BRACE (ie, row end)

  for (; (currToken && currToken->type != TokenType::SEMICOLON);
       currToken = currToken->sibling) {
    if (!currToken->sibling) {
      _retPosition = currToken; // for handling returns once row is done
    }
    switch (currToken->type) {
    case TokenType::INTEGER:
    case TokenType::IDENTIFIER: {
      auto *sym = getNodeSymbol(currToken);
      if (sym && sym->identifierType == TokenType::FUNCTION) {
        _function = true;
      }
      displayToken(currToken, _tokenStr, _tail);
    } break;
    case TokenType::TRUE:
    case TokenType::FALSE:
    case TokenType::SINGLE_QUOTE:
    case TokenType::DOUBLE_QUOTE:
    case TokenType::STRING:
    case TokenType::CHAR_LITERAL:
    case TokenType::L_BRACKET:
    case TokenType::R_BRACKET: {
      displayToken(currToken, _tokenStr, _tail);
      break;
    }
    case TokenType::L_PAREN: {
      (_function) ? displayToken(currToken, _tokenStr, _tail)
                  : _holdStack.push(currToken);
      break;
    }
    case TokenType::R_PAREN: {
      if (_function) {
        displayToken(currToken, _tokenStr, _tail);
        _function = false;
      } else {
        _finished = false;
        while (!_finished) {
          topToken = _holdStack.top();
          if (topToken->type == TokenType::L_PAREN) {
            _holdStack.pop();
            _finished = true;
          } else {
            displayToken(topToken, _tokenStr, _tail);
            _holdStack.pop();
          }
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
      if (_holdStack.empty()) {
        _holdStack.push(currToken);
      } else {
        switch (currToken->type) {
        case TokenType::BOOLEAN_NOT: {
          _finished = false;
          while (!_finished) {
            if (!_holdStack.empty()) {
              topToken = _holdStack.top();
              if (topToken->type == TokenType::BOOLEAN_NOT) {
                displayToken(topToken, _tokenStr, _tail);
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
                displayToken(topToken, _tokenStr, _tail);
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
              if ((topToken->type == TokenType::BOOLEAN_NOT) ||
                  isNumericOperator(topToken->type)) {
                displayToken(topToken, _tokenStr, _tail);
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
                displayToken(topToken, _tokenStr, _tail);
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
                displayToken(topToken, _tokenStr, _tail);
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
    displayToken(topToken, _tokenStr, _tail);
    _holdStack.pop();
  }
  return (_retPosition ? _retPosition : currToken);
}
