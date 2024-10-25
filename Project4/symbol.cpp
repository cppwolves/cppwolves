#include "symbol.hpp"
#include "token_enum.hpp"
#include <cstdint>

Symbol::Symbol()
    : _idName(), _scope(INTMAX_MAX), _idType(TokenType::INVALID_TOKEN),
      _datatype(TokenType::INVALID_TOKEN), _isArray(false),
      _arraySize(INTMAX_MAX), _sibling(nullptr), _child(nullptr) {}

Symbol::Symbol(const std::string &identifierName, size_t scope,
               TokenType identifierType, TokenType datatype, bool isArray,
               size_t arraySize, Symbol *sibling, Symbol *child)
    : _idName(identifierName), _scope(scope), _idType(identifierType),
      _datatype(datatype), _isArray(isArray), _arraySize(arraySize),
      _sibling(sibling), _child(child) {}
