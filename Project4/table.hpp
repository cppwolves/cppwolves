#ifndef TABLE_HPP
#define TABLE_HPP

#include "cst.hpp"
#include "symbol.hpp"

class SymbolTable {
public:
  SymbolTable(CSTree *cst);
  Symbol *head() { return _head; };

private:
  void addChild(Symbol *symbol);
  void addSibling(Symbol *symbol) const;

  void parseCST(CSTree *cst);
  Symbol *parseFunction(TokenNode **root, size_t scope);
  Symbol *parseProcedure(TokenNode **root, size_t scope);
  Symbol *parseDatatype(TokenNode **root, size_t scope);
  void parseDeclarators(TokenNode **rootToken, Symbol* rootDeclarator);

  Symbol *parseParameterList(TokenNode **root, size_t scope);
  static size_t parseArraySize(TokenNode **root);

private:
  Symbol *_head;
  Symbol *_current;
};

#endif // TABLE_HPP
