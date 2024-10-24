#include "table.hpp"
#include "cst.hpp"
#include "token_enum.hpp"
#include "token_error.hpp"
#include "token_node.hpp"
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <stdexcept>

/*

Symbol:
  std::string &identifierName
  size_t scope
  TokenType identifierType
  TokenType datatype
  bool isArray
  size_t arraySize
  Symbol *sibling
  Symbol *child

*/

struct Scope {
  Scope(int _val, bool _used) : value(_val), used(_used) {}
  int value;
  bool used;
};

SymbolTable::SymbolTable(CSTree *cst) : _head(nullptr), _current(nullptr) {
  parseCST(cst);
}

void SymbolTable::parseCST(CSTree *cst) {
  TokenNode *currToken = cst->head();
  std::stack<Scope> scopeStack;
  scopeStack.emplace(0, true);
  size_t nextScope = 1;

  while (currToken) {
    if (isIdentifier(currToken->type)) {
      switch (currToken->type) {
      case TokenType::FUNCTION: {
        scopeStack.emplace(nextScope++, true);
        addChild(parseFunction(&currToken, scopeStack.top().value));
        // Skip past '{'
        currToken = currToken->child;
        break;
      }
      case TokenType::PROCEDURE: {
        scopeStack.emplace(nextScope++, true);
        addChild(parseProcedure(&currToken, scopeStack.top().value));
        // Skip past '{'
        currToken = currToken->child;
        break;
      }
      case TokenType::L_BRACE: {
        // Update next candidate scope
        scopeStack.emplace(nextScope++, false);
        break;
      }
      case TokenType::R_BRACE: {
        // Pop scope from the stack regardless of current scope
        Scope last = scopeStack.top();
        scopeStack.pop();
        if (!last.used) {
          // No symbol was added in this scope
          if (_current && last.value < _current->_scope) {
            // Set to max scope + 1
            nextScope = _current->_scope + 1;
          } else {
            // Set to last unused scope
            nextScope = last.value;
          }
        }
        break;
      }
      default: {
        if (isDataType(currToken->type)) {
          // This is a new scope (not function or procedure)
          if (!scopeStack.top().used) {
            scopeStack.top().used = true;
          }
          Symbol *datatype = parseDatatype(&currToken, scopeStack.top().value);
          addChild(datatype);
          if (currToken->type == TokenType::COMMA && currToken->sibling) {
            // This is a declarator list: int i, j, k; (pass `int i` and append
            // symbols of its siblings)
            parseDeclarators(&currToken, datatype);
            while (datatype->_sibling) {
              Symbol *next = datatype->_sibling;
              datatype->_sibling = next->_sibling;
              next->_sibling = nullptr;
              addChild(next);
            }
          }
        } else {
          // Move past token to get to the next child in the tree
          while (currToken->sibling) {
            currToken = currToken->sibling;
          }
        }
      }
      } // end switch type
    }
    currToken = currToken->child;
  }
}

Symbol *SymbolTable::parseFunction(TokenNode **root, size_t scope) {
  Symbol *symbol = nullptr;
  Symbol *paramList = nullptr;
  int state = 0;
  TokenNode *curr = *root;

  // function datatype+name ( paramList... )
  while (curr && curr->sibling) {
    if (isIdentifier(curr->type)) {
      switch (state) {
      case 0: {
        // function
        if (curr->type != TokenType::FUNCTION) {
          throwSyntaxError(curr, "Token must be of type \"FUNCTION\"");
        }
        state++;
        break;
      }
      case 1: {
        symbol = parseDatatype(&curr, scope);
        symbol->_idType = TokenType::FUNCTION;
        state++;
        break;
      }
      case 2: {
        // ( -> paramList... -> )
        paramList = parseParameterList(&curr, scope);
        symbol->_sibling = paramList;
        state++;
        break;
      }
      default: {
        if (curr->type == TokenType::R_PAREN) {
          break;
        } else {
          throwSyntaxError(curr, "Unexpected symbol state");
        }
      }
      }
    }
    if (curr->sibling) {
      curr = curr->sibling;
    }
  }

  if (!symbol) {
    throwSyntaxError(*root, "Error parsing symbol");
  }
  *root = curr;
  return symbol;
}

Symbol *SymbolTable::parseProcedure(TokenNode **root, size_t scope) {
  TokenType idType = TokenType::INVALID_TOKEN;
  TokenType datatype = TokenType::INVALID_TOKEN;
  std::string idName{};
  Symbol *paramList = nullptr;
  int state = 0;
  TokenNode *curr = *root;

  // procedure name ( void | paramList... )
  while (curr && curr->sibling) {
    if (isIdentifier(curr->type)) {
      switch (state) {
      case 0: {
        // procedure
        if (curr->type != TokenType::PROCEDURE) {
          throwSyntaxError(curr, "Token must be of type \"PROCEDURE\"");
        }
        idType = TokenType::PROCEDURE;
        datatype = TokenType::NOT_APPLICABLE;
        state++;
        break;
      }
      case 1: {
        // name
        idName = curr->lexeme;
        state++;
        break;
      }
      case 2: {
        // ( -> void | paramList... -> )
        TokenType nextType =
            curr->sibling ? curr->sibling->type : TokenType::INVALID_TOKEN;
        switch (nextType) {
        case TokenType::VOID:
        case TokenType::R_PAREN:
        case TokenType::INVALID_TOKEN:
          break;
        default: {
          paramList = parseParameterList(&curr, scope);
          state++;
          break;
        }
        } // end switch type
        break;
      }
      default: {
        throwSyntaxError(curr, "Unexpected symbol state");
      }
      } // end switch state
    }
    if (curr->sibling) {
      curr = curr->sibling;
    }
  }
  if (idType == TokenType::INVALID_TOKEN ||
      datatype == TokenType::INVALID_TOKEN || idName.empty()) {
    throwSyntaxError(*root, "Error parsing symbol");
  }
  *root = curr;
  return new Symbol(idName, scope, idType, datatype, false, 0, paramList);
}

void SymbolTable::parseDeclarators(TokenNode **rootToken,
                                      Symbol *rootDeclarator) {
  std::string idName{};
  TokenType idType = rootDeclarator->identifierType();
  TokenType datatype = rootDeclarator->datatype();
  bool isArray = rootDeclarator->isArray();
  size_t arraySize = rootDeclarator->arraySize();
  size_t scope = rootDeclarator->scope();
  TokenNode *currToken = *rootToken;
  Symbol *currDelcarator = rootDeclarator;

  while (currToken->type != TokenType::SEMICOLON) {
    if (isIdentifier(currToken->type)) {
      if (isKeyword(currToken->type)) {
        throwSyntaxError(currToken, "Unexpected keyword token.");
      }
      // Create a new symbol using the rootDeclarator data (except name)
      // and append it as a sibling of rootDeclarator
      currDelcarator->_sibling = new Symbol(currToken->lexeme, scope, idType,
                                            datatype, isArray, arraySize);
      currDelcarator = currDelcarator->_sibling;
    } else if (currToken->type != TokenType::COMMA) {
      throwSyntaxError(currToken,
                       "Unknown token; expected COMMA, got " +
                           std::string(typeToCString(currToken->type)) + ".");
    }
    currToken = currToken->sibling;
  }

  *rootToken = currToken;
}

Symbol *SymbolTable::parseDatatype(TokenNode **root, size_t scope) {
  TokenType idType = TokenType::INVALID_TOKEN;
  TokenType datatype = TokenType::INVALID_TOKEN;
  std::string idName{};
  bool isArray = false;
  size_t arraySize = 0;
  int state = 0;
  TokenNode *curr = *root;

  // datatype name <[<size>]>
  while (curr && curr->sibling) {
    if (isIdentifier(curr->type)) {
      switch (state) {
      case 0: {
        // datatype
        idType = TokenType::IDENTIFIER;
        datatype = curr->type;
        state++;
        break;
      }
      case 1: {
        // name
        idName = curr->lexeme;
        state++;
        break;
      }
      }
    } else if (curr->type == TokenType::L_BRACKET && state == 2) {
      // [<size>]
      isArray = true;
      arraySize = parseArraySize(&curr);
      state++;
    } else if (curr->type == TokenType::L_PAREN ||
               curr->type == TokenType::COMMA) {
      break;
    }
    curr = curr->sibling;
  }

  if (idType == TokenType::INVALID_TOKEN ||
      datatype == TokenType::INVALID_TOKEN || idName.empty()) {
    throwSyntaxError(*root, "Error parsing symbol");
  }

  *root = curr;
  return new Symbol(idName, scope, idType, datatype, isArray, arraySize);
}

Symbol *SymbolTable::parseParameterList(TokenNode **root, size_t scope) {
  Symbol *rootSymbol = nullptr;
  Symbol *currSymbol = nullptr;
  TokenNode *currToken = *root;
  while (currToken && currToken->sibling) {
    if (currToken->type == TokenType::R_PAREN) {
      break;
    }
    if (isDataType(currToken->type)) {
      Symbol *param = parseDatatype(&currToken, scope);
      if (!rootSymbol) {
        rootSymbol = param;
        currSymbol = rootSymbol;
      } else if (currSymbol) {
        currSymbol->_sibling = param;
        currSymbol = currSymbol->_sibling;
      } else {
        throwSyntaxError(*root, "Unexpected end of file");
      }
    } else if (currToken->type == TokenType::COMMA) {
      return parseParameterList(&currToken->sibling, scope);
    }
    if (currToken->sibling) {
      currToken = currToken->sibling;
    }
  }
  *root = currToken;
  return rootSymbol;
}

size_t SymbolTable::parseArraySize(TokenNode **root) {
  TokenNode *curr = *root;
  size_t size = 0;
  while (curr && curr->sibling) {
    if (curr->type == TokenType::R_BRACKET) {
      *root = curr;
      break;
    }
    if (curr->type == TokenType::INTEGER) {
      size = std::stoi(curr->lexeme);
    }
    curr = curr->sibling;
  }
  return size;
}

void SymbolTable::addChild(Symbol *symbol) {
  if (!_head) {
    _head = symbol;
    _current = _head;
  } else {
    _current->_child = symbol;
    _current = _current->_child;
  }
}

void SymbolTable::addSibling(Symbol *symbol) const {
  if (!_head) {
    throw std::runtime_error("Cannot add sibling to empty list");
  }
  _current->_sibling = symbol;
}
