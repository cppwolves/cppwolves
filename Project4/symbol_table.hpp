#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include "cst.hpp"
#include "symbol_table_list_node.hpp"
#include "token_enum.hpp"

class SymbolTable {
  typedef SymbolTableListNode ListNode;

public:
  SymbolTable(CSTree *cst);
  ~SymbolTable();

  ListNode *head() { return _tableHead; };
  ListNode *tail() { return _tableTail; };

  ListNode *lookUp(const std::string &identifierName) const;
  ListNode *lookUp(const std::string &identifierName, TokenType type) const;
  ListNode *lookUp(const std::string &identifierName, TokenType type,
                   size_t scope) const;
  ListNode *lookUp(const std::string &identifierName, size_t scope) const;

  bool containsInScope(const std::string &identifierName, size_t scope) const;
  bool containsInScope(const std::string &identifierName, TokenType type,
                       size_t scope) const;

private:
  void parseCST(CSTree *cst);
  void addNext(ListNode *symbol);
  void addParameter(ListNode *symbol);

  size_t parseArraySize(TokenNode **rootToken) const;
  ListNode *parseFunction(TokenNode **rootToken, size_t scope) const;
  ListNode *parseProcedure(TokenNode **rootToken, size_t scope) const;
  ListNode *parseDatatype(TokenNode **rootToken, size_t scope) const;
  ListNode *parseParameterList(TokenNode **rootToken, size_t scope) const;
  ListNode *parseDeclaratorList(TokenNode **rootToken,
                                ListNode *rootSymbol) const;

  // Exceptions
private:
  void validateFunctionNotDefined(const std::string &identifierName,
                                  TokenNode *token) const;
  void validateProcedureNotDefined(const std::string &identifierName,
                                   TokenNode *token) const;
  void validateVariableNotDefined(const std::string &identifierName,
                                  TokenNode *token, size_t scope) const;

private:
  ListNode *_tableHead;
  ListNode *_tableTail;
  ListNode *_currentSymbol;
};

#endif // SYMBOL_TABLE_HPP
