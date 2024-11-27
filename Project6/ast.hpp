#ifndef ASTREE_HPP
#define ASTREE_HPP

#include "ast_list_node.hpp"
#include "cst.hpp"
#include "symbol_table.hpp"
#include "symbol_table_list_node.hpp"
#include "token.hpp"
#include "token_enum.hpp"
#include "token_node.hpp"

class ASTree {
public:
  ASTree(CSTree *cTree, SymbolTable *symTable);

  ASTListNode *head() { return _head; }
  ASTListNode *tail() { return _tail; }

private:
  ASTListNode *_head{nullptr};
  ASTListNode *_tail{nullptr};

  ASTListNode *_previous{nullptr};
  ASTListNode *_current{nullptr};
  TokenNode *_currCNode;

  SymbolTable *symTable;
  CSTree *cTree;

private:
  void addNext(ASTListNode *next);
  void advance();
  bool isDelimiter(TokenType type);

  TokenNode *numPostfixConverter(TokenNode *&currToken,
                                 ASTListNode *&_tokenStr);
  TokenNode *boolPostfixConverter(TokenNode *&currToken,
                                  ASTListNode *&_tokenStr);
  void displayToken(TokenNode *currToken);
  void displayToken(TokenNode *currToken, ASTListNode *&_tokenStr,
                    ASTListNode *&_tail);

  ASTListNode *parseDeclaration();
  ASTListNode *parseBooleanExp();
  ASTListNode *parseAssignment();
  ASTListNode *parseFor();
  ASTListNode *parseCall();
  ASTListNode *parseReturn();
  ASTListNode *parsePrintf();
  void pushNextScope();
  void popScope();
  SymbolTableListNode *getNodeSymbol(TokenNode *tokenNode);

private:
  std::vector<size_t> _scopeStack{};
  size_t _nextScope{0};
};

#endif // ASTREE_HPP
