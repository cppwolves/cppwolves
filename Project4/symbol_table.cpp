#include "symbol_table.hpp"
#include "cst.hpp"
#include "symbol_table_list_node.hpp"
#include "token_enum.hpp"
#include "token_error.hpp"
#include "token_node.hpp"
#include <cassert>

typedef SymbolTableListNode SymbolNode;

struct Scope {
  Scope(int val, bool used) : value(val), used(used) {}
  int value;
  bool used;
};

SymbolTable::SymbolTable(CSTree *cst)
    : _tableHead(nullptr), _currentSymbol(nullptr) {
  parseCST(cst);
}

SymbolTable::~SymbolTable() {
  _currentSymbol = _tableHead;
  while (_currentSymbol) {
    SymbolNode *next = _currentSymbol->next();
    delete _currentSymbol;
    _currentSymbol = next;
  }
}

SymbolNode *SymbolTable::find(const std::string &identifierName, int scope,
                              TokenType type) const {
  bool allScopes = scope < 0;
  bool findFirst = type == TokenType::DEFAULT;
  SymbolNode *curr = _tableHead;

  while (curr) {
    if (allScopes || curr->scope == scope) {
      if (curr->identifierName == identifierName &&
          (findFirst || curr->identifierType == type)) {
        return curr;
      }
      if (curr->parameterList) {
        SymbolNode *param = curr->parameterList;
        while (param) {
          if (param->identifierName == identifierName &&
              param->identifierType == type) {
            return param;
          }
          param = param->next();
        }
      }
    }
    curr = curr->next();
  }

  return nullptr;
}

bool SymbolTable::contains(const std::string &identifierName, int scope,
                           TokenType type) const {
  bool allScopes = scope < 0;
  bool findFirst = type == TokenType::DEFAULT;
  SymbolNode *curr = _tableHead;

  while (curr) {
    if (allScopes || curr->scope == scope) {
      if (curr->identifierName == identifierName &&
          (findFirst || curr->identifierType == type)) {
        return true;
      }
      if (curr->parameterList) {
        SymbolNode *param = curr->parameterList;
        while (param) {
          if (param->identifierName == identifierName &&
              (findFirst || param->identifierType == type)) {
            return true;
          }
          param = param->next();
        }
      }
    }
    curr = curr->next();
  }

  return false;
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
        addNext(parseFunction(&currToken, scopeStack.top().value));
        // Skip past L_BRACE
        currToken = currToken->child;
        break;
      }
      case TokenType::PROCEDURE: {
        scopeStack.emplace(nextScope++, true);
        addNext(parseProcedure(&currToken, scopeStack.top().value));
        // Skip past L_BRACE
        currToken = currToken->child;
        break;
      }
      default: {
        if (isDataType(currToken->type)) {
          // This is a new scope (not function or procedure)
          if (!scopeStack.top().used) {
            scopeStack.top().used = true;
          }
          addNext(parseDatatype(&currToken, scopeStack.top().value));
          if (currToken->type == TokenType::COMMA && currToken->sibling) {
            // This must be a declarator list: int i, j, k
            addNext(parseDeclaratorList(&currToken, _currentSymbol));

            // Move _currentSymbol to the last symbol added
            _currentSymbol = _tableTail;
          }
        } else {
          // Ignore this statement and move past token to get to the next child
          // in the tree
          while (currToken->sibling) {
            currToken = currToken->sibling;
          }
        }
      }
      } // end switch type
    } else if (currToken->type == TokenType::L_BRACE) {
      // Update next candidate scope
      scopeStack.emplace(nextScope++, false);
    } else if (currToken->type == TokenType::R_BRACE) {
      // The current scope has ended
      Scope last = scopeStack.top();
      scopeStack.pop();

      if (!last.used) {
        // No symbol was added in the previous scope
        if (_currentSymbol && last.value < _currentSymbol->scope) {
          // Go to the next available scope
          nextScope = _currentSymbol->scope + 1;
        } else {
          // Reuse the last unused scope
          nextScope = last.value;
        }
      }
    }
    currToken = currToken->child;
  }

  // Only the global scope should be left on the stack
  assert(scopeStack.size() == 1 && scopeStack.top().value == 0 &&
         "Scope stack is not correctly algined");
}

SymbolNode *SymbolTable::parseFunction(TokenNode **rootToken,
                                       size_t scope) const {
  TokenNode *currToken = *rootToken;
  SymbolNode *symbol = nullptr;
  SymbolNode *paramList = nullptr;
  int state = 0;

  // function datatype+name ( paramList... )
  while (currToken && currToken->sibling) {
    if (isIdentifier(currToken->type)) {
      switch (state) {
      case 0: {
        // function
        if (currToken->type != TokenType::FUNCTION) {
          throwSyntaxError(currToken, "Token must be of type \"FUNCTION\"");
        }
        state++;
        break;
      }
      case 1: {
        // datatype+name
        symbol = parseDatatype(&currToken, scope);
        validateFunctionNotDefined(symbol->identifierName, *rootToken);
        symbol->identifierType = TokenType::FUNCTION;
        state++;
        break;
      }
      case 2: {
        // ( -> paramList... -> )
        paramList = parseParameterList(&currToken, scope);
        symbol->addParameter(paramList);
        state++;
        break;
      }
      default: {
        if (currToken->type == TokenType::R_PAREN) {
          break;
        } else {
          throwSyntaxError(currToken, "Unexpected symbol state");
        }
      }
      } // end switch state
    }
    // We may have hit the end of the token stream after parsing
    if (currToken->sibling) {
      currToken = currToken->sibling;
    }
  }

  if (!symbol) {
    throwSyntaxError(*rootToken, "Error parsing symbol");
  }
  *rootToken = currToken;
  return symbol;
}

SymbolNode *SymbolTable::parseProcedure(TokenNode **rootToken,
                                        size_t scope) const {
  TokenNode *currToken = *rootToken;

  // Symbol data
  TokenType idType = TokenType::INVALID_TOKEN;
  TokenType datatype = TokenType::INVALID_TOKEN;
  std::string idName{};
  SymbolNode *paramList = nullptr;
  int state = 0;

  // procedure name ( void | paramList... )
  while (currToken && currToken->sibling) {
    if (isIdentifier(currToken->type)) {
      switch (state) {
      case 0: {
        // procedure
        if (currToken->type != TokenType::PROCEDURE) {
          throwSyntaxError(currToken, "Token must be of type \"PROCEDURE\"");
        }
        idType = TokenType::PROCEDURE;
        datatype = TokenType::NOT_APPLICABLE;
        state++;
        break;
      }
      case 1: {
        // name
        idName = currToken->lexeme;
        validateProcedureNotDefined(idName, *rootToken);
        state++;
        break;
      }
      case 2: {
        // ( -> void | paramList... -> )
        TokenType nextType = currToken->sibling ? currToken->sibling->type
                                                : TokenType::INVALID_TOKEN;
        switch (nextType) {
        case TokenType::VOID:
        case TokenType::R_PAREN:
        case TokenType::INVALID_TOKEN:
          break;
        default: {
          paramList = parseParameterList(&currToken, scope);
          state++;
          break;
        }
        } // end switch type
        break;
      }
      default: {
        throwSyntaxError(currToken, "Unexpected symbol state");
      }
      } // end switch state
    }
    if (currToken->sibling) {
      currToken = currToken->sibling;
    }
  }
  if (idType == TokenType::INVALID_TOKEN ||
      datatype == TokenType::INVALID_TOKEN || idName.empty()) {
    throwSyntaxError(*rootToken, "Error parsing symbol");
  }
  *rootToken = currToken;

  SymbolNode *symbol =
      new SymbolNode(idName, scope, idType, datatype, false, 0);
  symbol->addParameter(paramList);

  return symbol;
}

SymbolNode *SymbolTable::parseDeclaratorList(TokenNode **rootToken,
                                             SymbolNode *rootDeclarator) const {
  TokenNode *currToken = *rootToken;
  SymbolNode *headDeclarator = nullptr;
  SymbolNode *currDeclarator = nullptr;

  // Symbol data
  std::string idName{};
  TokenType idType = rootDeclarator->identifierType;
  TokenType datatype = rootDeclarator->datatype;
  bool isArray = rootDeclarator->isArray;
  size_t arraySize = rootDeclarator->arraySize;
  size_t scope = rootDeclarator->scope;

  while (currToken->type != TokenType::SEMICOLON) {
    if (isIdentifier(currToken->type)) {
      if (isKeyword(currToken->type)) {
        throwSyntaxError(currToken, "Unexpected keyword token.");
      }
      validateVariableNotDefined(currToken->lexeme, currToken, scope);

      // Create the next symbol using the rootDeclarator data (except name)
      // and add it to the declarator list
      SymbolNode *next = new SymbolNode(currToken->lexeme, scope, idType,
                                        datatype, isArray, arraySize);

      if (!headDeclarator) {
        headDeclarator = next;
        currDeclarator = headDeclarator;
      } else {
        currDeclarator = currDeclarator->link(next);
      }

    } else if (currToken->type != TokenType::COMMA) {
      throwSyntaxError(currToken,
                       "Unknown token; expected COMMA, got " +
                           std::string(typeToCString(currToken->type)) + ".");
    }
    currToken = currToken->sibling;
  }

  *rootToken = currToken;
  return headDeclarator;
}

SymbolNode *SymbolTable::parseDatatype(TokenNode **rootToken,
                                       size_t scope) const {
  TokenNode *currToken = *rootToken;

  // Symbol data
  TokenType idType = TokenType::INVALID_TOKEN;
  TokenType datatype = TokenType::INVALID_TOKEN;
  std::string idName{};
  bool isArray = false;
  size_t arraySize = 0;
  int state = 0;

  // datatype name <[<size>]>
  while (currToken && currToken->sibling) {
    if (isIdentifier(currToken->type)) {
      switch (state) {
      case 0: {
        // datatype
        idType = TokenType::DATATYPE;
        datatype = currToken->type;
        state++;
        break;
      }
      case 1: {
        // name
        idName = currToken->lexeme;
        validateVariableNotDefined(idName, *rootToken, scope);
        state++;
        break;
      }
      } // end switch state
    } else if (currToken->type == TokenType::L_BRACKET && state == 2) {
      // [<size>]
      isArray = true;
      arraySize = parseArraySize(&currToken);
      state++;
    } else if (currToken->type == TokenType::L_PAREN ||
               currToken->type == TokenType::COMMA) {
      break;
    }
    currToken = currToken->sibling;
  }

  if (idType == TokenType::INVALID_TOKEN ||
      datatype == TokenType::INVALID_TOKEN || idName.empty()) {
    throwSyntaxError(*rootToken, "Error parsing symbol");
  }

  *rootToken = currToken;
  return new SymbolNode(idName, scope, idType, datatype, isArray, arraySize);
}

SymbolNode *SymbolTable::parseParameterList(TokenNode **rootToken,
                                            size_t scope) const {
  TokenNode *currToken = *rootToken;
  SymbolNode *paramList = nullptr;
  SymbolNode *currParam = nullptr;

  while (currToken && currToken->sibling) {
    if (currToken->type == TokenType::R_PAREN) {
      break;
    }
    if (isDataType(currToken->type)) {
      SymbolNode *next = parseDatatype(&currToken, scope);
      if (!paramList) {
        paramList = next;
        currParam = paramList;
      } else {
        currParam = currParam->link(next);
      }
    } else if (currToken->type == TokenType::COMMA) {
      // Parse and link the remaining list of parameters
      currParam->link(parseParameterList(&currToken->sibling, scope));
      return paramList;
    }
    if (currToken->sibling) {
      currToken = currToken->sibling;
    }
  }
  *rootToken = currToken;
  return paramList;
}

size_t SymbolTable::parseArraySize(TokenNode **rootToken) const {
  TokenNode *currToken = *rootToken;
  size_t size = 0;

  while (currToken && currToken->sibling) {
    if (currToken->type == TokenType::R_BRACKET) {
      *rootToken = currToken;
      break;
    }
    if (currToken->type == TokenType::INTEGER) {
      size = std::stoi(currToken->lexeme);
    }
    currToken = currToken->sibling;
  }
  return size;
}

void SymbolTable::addNext(SymbolNode *symbol) {
  if (!_tableHead) {
    _tableHead = symbol;
    _currentSymbol = _tableHead;
  } else {
    _currentSymbol = _currentSymbol->link(symbol);
  }
  if (!_currentSymbol->next()) {
    _tableTail = _currentSymbol;
  }
}

void SymbolTable::validateFunctionNotDefined(const std::string &identifierName,
                                             TokenNode *token) const {
  if (find(identifierName, -1, TokenType::FUNCTION)) {
    throwError(token,
               "function \"" + identifierName + "\" is already defined.");
  }
}

void SymbolTable::validateProcedureNotDefined(const std::string &identifierName,
                                              TokenNode *token) const {
  if (find(identifierName, -1, TokenType::PROCEDURE)) {
    throwError(token,
               "procedure \"" + identifierName + "\" is already defined.");
  }
}

void SymbolTable::validateVariableNotDefined(const std::string &identifierName,
                                             TokenNode *token,
                                             size_t scope) const {
  if (contains(identifierName, 0)) {
    throwError(token, "variable \"" + identifierName +
                          "\" is already defined globally.");
  } else if (contains(identifierName, scope)) {
    throwError(token, "variable \"" + identifierName +
                          "\" is already defined locally.");
  }
}
