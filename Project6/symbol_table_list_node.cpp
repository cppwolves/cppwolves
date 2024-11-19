#include "symbol_table_list_node.hpp"
#include "token_enum.hpp"

SymbolTableListNode::SymbolTableListNode(const std::string &identifierName,
                                         size_t scope, TokenType identifierType,
                                         TokenType datatype, bool isArray,
                                         size_t arraySize)
    : identifierName(identifierName), scope(scope),
      identifierType(identifierType), datatype(datatype), isArray(isArray),
      arraySize(arraySize), address(0) {} // Initialize address to 0

SymbolTableListNode::~SymbolTableListNode() {
  SymbolTableListNode *curr = this->parameterList;
  // Delete any parameters
  while (curr) {
    SymbolTableListNode *next = curr->next();
    delete curr;
    curr = next;
  }

  this->unlink();
}

SymbolTableListNode *SymbolTableListNode::link(SymbolTableListNode *symbol) {
  return (SymbolTableListNode *)ListNode<SymbolTableListNode>::link(
      (ListNode *)symbol);
}

SymbolTableListNode *
SymbolTableListNode::addParameter(SymbolTableListNode *symbol) {
  if (!this->parameterList) {
    this->parameterList = symbol;
  } else {
    SymbolTableListNode *curr = this->parameterList;
    while (curr && curr->next()) {
      curr = curr->next();
    }
    curr->link(symbol);
  }
  return symbol;
}

SymbolTableListNode *
SymbolTableListNode::removeParameter(const std::string &identiferName) {
  SymbolTableListNode *curr = this->parameterList;
  while (curr && curr->identifierName != identifierName) {
    curr = curr->next();
  }
  if (curr) {
    curr->unlink();
    return curr;
  }
  return nullptr;
}
