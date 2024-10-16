#ifndef CSTREE_HPP
#define CSTREE_HPP

#include <stack>
#include <vector>

#include "token.hpp"
#include "token_enum.hpp"
#include "token_node.hpp"

class CSTree {
public:
  CSTree(std::vector<Token> &tokens);
  TokenNode *head() { return _head; }

private:
  TokenNode *_head;

  TokenNode *_previous;
  TokenNode *_current;

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
  void addSiblingAndAdvance(TokenNode *node);
  void addChildAndAdvance(TokenNode *node);
  void handleOpenCloseDelimiters(TokenNode *node);
  bool isOperand(TokenNode *token);
  void revertState(TokenNode *node);

private:
  // Exceptions
  void throwSyntaxError(TokenNode *node, const std::string &message);
  void throwInvalidProcedureNameError(TokenNode *node);
  void throwInvalidFunctionNameError(TokenNode *node);
  void throwInvalidVariableNameError(TokenNode *node);
  void throwArrayNegativeDeclarationError(TokenNode *node);
  void throwMissingInitializationExpressionError(TokenNode *node);
  void throwMissingBooleanExpressionError(TokenNode *node);
  void throwMissingNumericalExpressionError(TokenNode *node);
  void throwMissingOpeningParenthesisError(TokenNode *node);
  void throwMissingClosingParenthesisError(TokenNode *node);
  void throwMissingOpeningBraceError(TokenNode *node);
  void throwMissingClosingBraceError(TokenNode *node);
  void throwMissingOpeningBracketError(TokenNode *node);
  void throwMissingClosingBracketError(TokenNode *node);
  void throwMissingSemicolonError(TokenNode *node);
  void throwMissingIdentifierError(TokenNode *node);
  void throwMissingIdentifierListError(TokenNode *node);
  void throwMissingDatatypeError(TokenNode *node);
  void throwMissingAssignmentOperatorError(TokenNode *node);
  void throwMissingBooleanOperatorError(TokenNode *node);
  void throwMissingPlusOperatorError(TokenNode *node);
  void throwMissingMinusOperatorError(TokenNode *node);
  void throwMissingParameterListError(TokenNode *node);

private:
  bool _operandFlag;
  bool _chainCheck;
  bool _paramCheck;
  std::stack<TokenType> _openStack{};
};

#endif // CSTREE_HPP
