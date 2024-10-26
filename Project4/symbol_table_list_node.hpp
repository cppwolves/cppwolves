#ifndef SYBMOL_TABLE_LIST_NODE_HPP

#include "token_enum.hpp"

#include <string>

class SymbolTableListNode {
public:
  SymbolTableListNode() = default;
  SymbolTableListNode(const std::string &identifierName, size_t scope,
                      TokenType identifierType, TokenType datatype,
                      bool isArray, size_t arraySize);
  ~SymbolTableListNode();

  SymbolTableListNode *link(SymbolTableListNode *symbol);
  SymbolTableListNode *unlink();

  SymbolTableListNode *addParameter(SymbolTableListNode *symbol);
  SymbolTableListNode *removeParameter(const std::string &identiferName);

public:
  bool isArray{false};

  size_t scope{};
  size_t arraySize{0};

  std::string identifierName{};

  TokenType identifierType{TokenType::INVALID_TOKEN};
  TokenType datatype{TokenType::INVALID_TOKEN};

  SymbolTableListNode *next{nullptr};
  SymbolTableListNode *previous{nullptr};
  SymbolTableListNode *parameterList{nullptr};
};

#endif // !SYBMOL_TABLE_NODE_HPP
