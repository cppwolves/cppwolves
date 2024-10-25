#ifndef TABLE_HPP
#define TABLE_HPP

#include "cst.hpp"
#include "symbol.hpp"
#include "token_enum.hpp"

class SymbolTable {
public:
  SymbolTable(CSTree *cst);

  Symbol *head() { return _head; };
  Symbol *lookUp(const std::string &identifierName);
  Symbol *lookUp(const std::string &identifierName, TokenType type);
  Symbol *lookUp(const std::string &identifierName, TokenType type,
                 size_t scope);
  Symbol *lookUp(const std::string &identifierName, size_t scope);

  bool containsInScope(const std::string &identifierName, size_t scope);
  bool containsInScope(const std::string &identifierName, TokenType type,
                       size_t scope);

private:
  void addChild(Symbol *symbol);
  void addSibling(Symbol *symbol) const;

  void parseCST(CSTree *cst);
  Symbol *parseFunction(TokenNode **root, size_t scope);
  Symbol *parseProcedure(TokenNode **root, size_t scope);
  Symbol *parseDatatype(TokenNode **root, size_t scope);
  void parseDeclarators(TokenNode **rootToken, Symbol *rootDeclarator);

  Symbol *parseParameterList(TokenNode **root, size_t scope);
  static size_t parseArraySize(TokenNode **root);

private:
  void validateFunctionNotDefined(const std::string &identifierName,
                             TokenNode *token);
  void validateProcedureNotDefined(const std::string &identifierName,
                              TokenNode *token);
  void validateVariableNotDefined(const std::string &identifierName,
                             TokenNode *token, size_t scope);

private:
  Symbol *_head;
  Symbol *_current;
};

#endif // TABLE_HPP
