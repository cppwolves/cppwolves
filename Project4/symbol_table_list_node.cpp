#include "symbol_table_list_node.hpp"
#include "token_enum.hpp"

SymbolTableListNode::SymbolTableListNode(const std::string &identifierName,
                                         size_t scope, TokenType identifierType,
                                         TokenType datatype, bool isArray,
                                         size_t arraySize)
    : identifierName(identifierName), scope(scope),
      identifierType(identifierType), datatype(datatype), isArray(isArray),
      arraySize(arraySize) {}

SymbolTableListNode::~SymbolTableListNode() {
  SymbolTableListNode *curr = this->parameterList;
  // Delete any parameters
  while (curr) {
    SymbolTableListNode *next = curr->next;
    delete curr;
    curr = next;
  }

  this->unlink();
}

SymbolTableListNode *SymbolTableListNode::link(SymbolTableListNode *symbol) {
  if (!symbol) {
    this->next = nullptr;
    return nullptr;
  }

  // Disconnect the list to insert the new list
  SymbolTableListNode *end = this->next;

  // Connect the end of the old and start of the new list
  this->next = symbol;
  symbol->previous = this;

  SymbolTableListNode *next = symbol->next;

  // Move to the last node of the new list.
  while (next && next->next) {
    next = next->next;
  }
  // Connect the end of the new to the start of the old list
  if (next) {
    next->next = end;
  }
  if (end) {
    end->previous = next;
  }

  return symbol;
}

SymbolTableListNode *SymbolTableListNode::unlink() {
  SymbolTableListNode *previous = this->previous;
  SymbolTableListNode *next = this->next;
  if (previous) {
    next = previous->link(next);
  } else if (next) {
    next->previous = nullptr;
  }
  return next;
}

SymbolTableListNode *
SymbolTableListNode::addParameter(SymbolTableListNode *symbol) {
  if (!this->parameterList) {
    this->parameterList = symbol;
  } else {
    SymbolTableListNode *curr = this->parameterList;
    while (curr && curr->next) {
      curr = curr->next;
    }
    curr->link(symbol);
  }
  return symbol;
}

SymbolTableListNode *
SymbolTableListNode::removeParameter(const std::string &identiferName) {
  SymbolTableListNode *curr = this->parameterList;
  while (curr && curr->identifierName != identifierName) {
    curr = curr->next;
  }
  if (curr) {
    curr->unlink();
    return curr;
  }
  return nullptr;
}
