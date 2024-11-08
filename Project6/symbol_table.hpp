#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include "cst.hpp"
#include "list.hpp"
#include "symbol_table_list_node.hpp"
#include "token_enum.hpp"

class SymbolTable : public List<SymbolTableListNode> {
public:
  typedef SymbolTableListNode SymbolNode;

public:
  SymbolTable(CSTree &cst);
  virtual ~SymbolTable() override;

  virtual SymbolNode *head() override { return _tableHead; };
  virtual SymbolNode *tail() override { return _tableTail; };

  SymbolNode *find(const std::string &identifierName, int scope = -1,
                   TokenType type = TokenType::DEFAULT) const;

  bool contains(const std::string &identifierName, int scope = -1,
                TokenType type = TokenType::DEFAULT) const;

private:
  void parseCST(CSTree &cst);
  void addNext(SymbolNode *symbol);
  void addParameter(SymbolNode *symbol);

  size_t parseArraySize(TokenNode **rootToken) const;
  SymbolNode *parseFunction(TokenNode **rootToken, size_t scope) const;
  SymbolNode *parseProcedure(TokenNode **rootToken, size_t scope) const;
  SymbolNode *parseDatatype(TokenNode **rootToken, size_t scope) const;
  SymbolNode *parseParameterList(TokenNode **rootToken, size_t scope) const;
  SymbolNode *parseDeclaratorList(TokenNode **rootToken,
                                  SymbolNode *rootSymbol) const;

  // Exceptions
private:
  void validateFunctionNotDefined(const std::string &identifierName,
                                  TokenNode *token) const;
  void validateProcedureNotDefined(const std::string &identifierName,
                                   TokenNode *token) const;
  void validateVariableNotDefined(const std::string &identifierName,
                                  TokenNode *token, size_t scope) const;

private:
  SymbolNode *_tableHead;
  SymbolNode *_tableTail;
  SymbolNode *_currentSymbol;
};

#endif // SYMBOL_TABLE_HPP
