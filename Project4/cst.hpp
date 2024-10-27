#ifndef CSTREE_HPP
#define CSTREE_HPP

#include <stack>
#include <vector>

#include "list.hpp"
#include "token.hpp"
#include "token_enum.hpp"
#include "token_node.hpp"

class CSTree : public List<TokenNode> {
public:
  CSTree(std::vector<Token> &tokens);
  ~CSTree() override;

  TokenNode *head() override { return _head; }
  TokenNode *tail() override { return _tail; }

private:
  TokenNode *_head{nullptr};
  TokenNode *_tail{nullptr};

  TokenNode *_previous{nullptr};
  TokenNode *_current{nullptr};

  std::vector<Token>::iterator _nIt;

private:
  // States
  void isFor();
  void isWhile();
  void isFunction();
  void isDatatypeSpecifier();
  void isProcedure();
  void isMain();
  bool isInitializationExpression();
  bool isBooleanExpression();
  bool isNumericalExpression();
  bool isExpression();
  bool isParameterList();
  bool isIdentifierList(); // and arrays

private:
  // Helpers
  TokenNode *getNextToken();
  void addSiblingAndAdvance(TokenNode *node);
  void addChildAndAdvance(TokenNode *node);
  void handleOpenCloseDelimiters(TokenNode *node);
  bool isOperand(TokenNode *token);
  void revertState(TokenNode *node);
  bool isNewLine() const;

private:
  bool _operandFlag{};
  std::stack<TokenType> _openStack{};
};

#endif // CSTREE_HPP
