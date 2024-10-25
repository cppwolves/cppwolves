#include "token_node.hpp"

#include <stdexcept>

static void throwError(TokenNode *node, const std::string &message) {
  throw std::runtime_error("Error on line " +
                           std::to_string(node->lineNumber) + ": " + message);
}

static void throwSyntaxError(TokenNode *node, const std::string &message) {
  throw std::runtime_error("Syntax error on line " +
                           std::to_string(node->lineNumber) + ": " + message);
}

static void throwSyntaxError(Token token, const std::string &message) {
  throw std::runtime_error("Syntax error on line " +
                           std::to_string(token.lineNumber) + ": " + message);
}

static void throwInvalidProcedureNameError(TokenNode *node) {
  throwSyntaxError(node, "reserved word \"" + node->lexeme +
                             "\" cannot be used as a procedure name.");
}

static void throwInvalidFunctionNameError(TokenNode *node) {
  throwSyntaxError(node, "reserved word \"" + node->lexeme +
                             "\" cannot be used for the name of a function.");
}

static void throwInvalidVariableNameError(TokenNode *node) {
  throwSyntaxError(node, "reserved word \"" + node->lexeme +
                             "\" cannot be used for the name of a variable.");
}

static void throwArrayNegativeDeclarationError(TokenNode *node) {
  throwSyntaxError(node, "array declaration size must be a positive integer.");
}

static void throwMissingInitializationExpressionError(TokenNode *node) {
  throwSyntaxError(node, "missing initialization expression.");
}

static void throwMissingBooleanExpressionError(TokenNode *node) {
  throwSyntaxError(node, "missing boolean expression.");
}

static void throwMissingNumericalExpressionError(TokenNode *node) {
  throwSyntaxError(node, "missing numerical expression.");
}

static void throwMissingOpeningParenthesisError(TokenNode *node) {
  throwSyntaxError(node, "missing opening parenthesis.");
}

static void throwMissingClosingParenthesisError(TokenNode *node) {
  throwSyntaxError(node, "missing closing paranthesis.");
}

static void throwMissingOpeningBraceError(TokenNode *node) {
  throwSyntaxError(node, "missing opening brace.");
}

static void throwMissingClosingBraceError(TokenNode *node) {
  throwSyntaxError(node, "missing closing brace.");
}

static void throwMissingOpeningBracketError(TokenNode *node) {
  throwSyntaxError(node, "missing opening bracket.");
}

static void throwMissingClosingBracketError(TokenNode *node) {
  throwSyntaxError(node, "missing closing bracket.");
}

static void throwMissingSemicolonError(TokenNode *node) {
  throwSyntaxError(node, "missing semicolon.");
}

static void throwMissingIdentifierError(TokenNode *node) {
  throwSyntaxError(node, "missing identifier.");
}

static void throwMissingIdentifierListError(TokenNode *node) {
  throwSyntaxError(node, "missing identifier list.");
}

static void throwMissingDatatypeError(TokenNode *node) {
  throwSyntaxError(node, "missing datatype.");
}

static void throwMissingAssignmentOperatorError(TokenNode *node) {
  throwSyntaxError(node, "missing assignment operator.");
}

static void throwMissingBooleanOperatorError(TokenNode *node) {
  throwSyntaxError(node, "missing boolean operator.");
}

static void throwMissingPlusOperatorError(TokenNode *node) {
  throwSyntaxError(node, "missing plus operator.");
}

static void throwMissingMinusOperatorError(TokenNode *node) {
  throwSyntaxError(node, "missing minus operator.");
}

static void throwMissingParameterListError(TokenNode *node) {
  throwSyntaxError(node, "missing parameter list.");
}

static void throwUnterminatedStringError(TokenNode *node) {
  throwSyntaxError(node, "unterminated string quote.");
}

static void throwUnterminatedStringError(Token token) {
  throwSyntaxError(token, "unterminated string quote.");
}

