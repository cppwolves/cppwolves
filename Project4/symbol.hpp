#ifndef SYBMOL_HPP

#include "token_enum.hpp"

#include <string>

class SymbolTable;

class Symbol {
public:
  Symbol(const std::string &identifierName, size_t scope,
         TokenType identifierType, TokenType datatype, bool isArray,
         size_t arraySize, Symbol *sibling = nullptr, Symbol *child = nullptr);

  const bool isArray() const { return _isArray; };

  const size_t scope() const { return _scope; };
  const size_t arraySize() const { return _arraySize; };

  const std::string &identifierName() const { return _idName; };

  TokenType identifierType() const { return _idType; };
  TokenType datatype() const { return _datatype; };

  const Symbol *sibling() const { return _sibling; };
  const Symbol *child() const { return _child; };

private:
  Symbol();

private:
  bool _isArray;

  size_t _scope;
  size_t _arraySize;

  std::string _idName;

  TokenType _idType;
  TokenType _datatype;

  Symbol *_sibling;
  Symbol *_child;

  friend SymbolTable;
};

#endif // !SYBMOL_HPP
