#include "cst.hpp"

#include "token_enum.hpp"
#include "token_error.hpp"
#include "token_node.hpp"

CSTree::CSTree(std::vector<Token> &tokens) {
  if (tokens.empty()) {
    // return nullptr;
    // deal with, what behavior?
  }

  _head = new TokenNode(tokens.at(0));
  _current = _head;

  _nIt = tokens.begin();

  for (_nIt++; _nIt != tokens.end(); _nIt++) {
    // First, check the next node
    TokenNode *next = new TokenNode(*_nIt);
    switch (next->type) {
    // The next token is a delimiter
    case TokenType::L_BRACE:
    case TokenType::R_BRACE:
    case TokenType::ELSE: {
      addChildAndAdvance(next);
      break;
    }
    default:
      // Default to the current node
      switch (_current->type) {
      case TokenType::FOR: {
        isFor();
        break;
      }
      case TokenType::WHILE: {
        isWhile();
        break;
      }
      case TokenType::FUNCTION: {
        isFunction();
        break;
      }
      case TokenType::PROCEDURE: {
        isProcedure();
        break;
      }
      case TokenType::INT:
      case TokenType::CHAR:
      case TokenType::BOOL: {
        isDatatypeSpecifier();
        break;
      }
      case TokenType::L_BRACE:
      case TokenType::R_BRACE:
      case TokenType::ELSE:
      case TokenType::SEMICOLON: {
        addChildAndAdvance(next);
        break;
      }
      default: {
        addSiblingAndAdvance(next);
        break;
      }
      }
    }
  }
}

void CSTree::addSiblingAndAdvance(TokenNode *node) {
  handleOpenCloseDelimiters(node);
  _current->sibling = node;
  _previous = _current;
  _current = node;
}

void CSTree::addChildAndAdvance(TokenNode *node) {
  handleOpenCloseDelimiters(node);
  _current->child = node;
  _previous = _current;
  _current = node;
}

void CSTree::handleOpenCloseDelimiters(TokenNode *node) {
  switch (node->type) {
  case TokenType::L_BRACE: {
    _openStack.push(TokenType::L_BRACE);
    break;
  }
  case TokenType::L_PAREN: {
    _openStack.push(TokenType::L_PAREN);
    break;
  }
  case TokenType::R_PAREN: {
    if (_openStack.top() != TokenType::L_PAREN) {
      throwMissingOpeningParenthesisError(node);
    }
    _openStack.pop();
    break;
  }
  case TokenType::L_BRACKET: {
    _openStack.push(TokenType::L_BRACKET);
    break;
  }
  case TokenType::R_BRACKET: {
    if (_openStack.top() != TokenType::L_BRACKET) {
      throwMissingOpeningBracketError(node);
    }
    _openStack.pop();
    break;
  }
  }
}

void CSTree::isFor() {
  TokenNode *lParen = new TokenNode(*_nIt++);

  if (lParen->type != TokenType::L_PAREN) {
    throwMissingOpeningParenthesisError(lParen);
  }

  addSiblingAndAdvance(lParen);

  if (!isInitializationExpression()) {
    throwMissingInitializationExpressionError(lParen);
  }

  TokenNode *semiColon = new TokenNode(*_nIt++);
  if (semiColon->type != TokenType::SEMICOLON) {
    throwMissingSemicolonError(semiColon);
  }

  addSiblingAndAdvance(semiColon);
  if (!isBooleanExpression()) {
    throwMissingBooleanExpressionError(semiColon);
  }

  TokenNode *semiColon2 = new TokenNode(*_nIt++);
  if (semiColon2->type != TokenType::SEMICOLON) {
    throwMissingSemicolonError(semiColon2);
  }

  addSiblingAndAdvance(semiColon2);

  if (!isNumericalExpression()) {
    throwMissingNumericalExpressionError(semiColon2);
  }

  TokenNode *rParen = new TokenNode(*_nIt);
  if (rParen->type != TokenType::R_PAREN) {
    throwMissingClosingParenthesisError(rParen);
  }
  if (_openStack.top() != TokenType::L_PAREN) {
    throwMissingClosingParenthesisError(rParen);
  }

  addSiblingAndAdvance(rParen);
}

void CSTree::isWhile() {
  TokenNode *lParen = new TokenNode(*_nIt++);

  if (lParen->type != TokenType::L_PAREN) {
    throwMissingOpeningParenthesisError(lParen);
  }

  addSiblingAndAdvance(lParen);

  if (!isBooleanExpression()) {
    throwMissingBooleanExpressionError(lParen);
  }

  TokenNode *rParen = new TokenNode(*_nIt);
  if (rParen->type != TokenType::R_PAREN) {
    throwMissingClosingParenthesisError(rParen);
  }
  if (_openStack.top() != TokenType::L_PAREN) {
    throwMissingClosingParenthesisError(rParen);
  }

  addSiblingAndAdvance(rParen);
}

bool CSTree::isInitializationExpression() {
  // needs to handle (INT, CHAR, DATATYPE, etc before checking identifier)
  // currently, does not
  // should be before called? check BNF

  // must start with identifier
  TokenNode *identifier = new TokenNode(*_nIt++);

  if (!isIdentifier(identifier->type)) {
    throwMissingIdentifierError(identifier);
  }

  addSiblingAndAdvance(identifier);

  // followed by assignment operator
  TokenNode *assignmentOp = new TokenNode(*_nIt++);
  if (assignmentOp->type != TokenType::ASSIGNMENT_OPERATOR) {
    throwMissingAssignmentOperatorError(assignmentOp);
  }

  addSiblingAndAdvance(assignmentOp);

  // can be string OR expression
  TokenNode *unknown = new TokenNode(*_nIt++);
  if (unknown->type == TokenType::STRING) {
    _current->sibling = unknown;
    _current = unknown;
    return true;
  }

  // FOR TESTING, just to get single number
  addSiblingAndAdvance(unknown);

  // else, must be an expression to be valid
  // isExpression(); create
  return true;
}

bool CSTree::isBooleanExpression() {
  TokenNode *holderNode = _current;
  auto holderIt = _nIt;

  // check if single operand,
  // if true AND next token isnt relational op - switch to boolExp case
  // checks
  if (_nIt->type == TokenType::L_PAREN) {
    addSiblingAndAdvance(new TokenNode(*_nIt++));
    if (isBooleanExpression()) {
      if (_nIt->type == TokenType::R_PAREN) {
        addSiblingAndAdvance(new TokenNode(*_nIt++));
        if (isBooleanOperator(_nIt->type)) {
          addSiblingAndAdvance(new TokenNode(*_nIt++));
        }
        if (_nIt->type == TokenType::SEMICOLON ||
            _nIt->type == TokenType::R_PAREN) {
          return true;
        }
        return isBooleanExpression();
      }
      return true;
    } else {
      revertState(holderNode);
      return false;
    }
  }
  if (isNumericalExpression() ||
      (!_operandFlag || (_operandFlag && isRelationalOperator(_nIt->type)))) {
    TokenNode *next = new TokenNode(*_nIt++);

    addSiblingAndAdvance(next);
    if (next->type == TokenType::TRUE || next->type == TokenType::FALSE) {
      return true;
    }

    if (!isNumericalExpression() && !isBooleanExpression()) {
      throwMissingNumericalExpressionError(next);
    }
    if (isBooleanOperator(_nIt->type)) {
      addSiblingAndAdvance(new TokenNode(*_nIt++));
      return isBooleanExpression();
    }
    return true;
  } else {
    // clear tree of num, fix this mess
    revertState(holderNode);
    TokenNode *first = new TokenNode(*_nIt++);

    switch (first->type) {
    case TokenType::FALSE:
    case TokenType::TRUE: {
      // boolean TRUE or FALSE

      addSiblingAndAdvance(first);
      return true;
    }
    case TokenType::IDENTIFIER: {
      addSiblingAndAdvance(first);
      TokenNode *second = new TokenNode(*_nIt++);

      // identifier & boolean op & boolean expression (recursion)
      if (isBooleanOperator(second->type)) {
        addSiblingAndAdvance(second);
        if (!isBooleanExpression()) {
          throwMissingBooleanExpressionError(second);
        }
        return true;
      }

      // identifier
      else {
        _nIt--; // un-gets second
        delete second;
        return true;
      }
    }
    case TokenType::L_PAREN: {
      // L-Paren & identifier & boolean op & boolean expression (recursion)
      // & R-Paren

      // add L-Paren to tree

      addSiblingAndAdvance(first);

      TokenNode *next = new TokenNode(*_nIt++);
      if (next->type != TokenType::IDENTIFIER) {
        throwMissingIdentifierError(next);
      }

      // add identifier to tree

      addSiblingAndAdvance(next);

      TokenNode *boolOp = new TokenNode(*_nIt++);
      if (!isBooleanOperator(boolOp->type)) {
        throwMissingBooleanExpressionError(boolOp);
      }

      // add boolean operator to tree

      addSiblingAndAdvance(boolOp);

      // handles inner boolean expression itself
      if (!isBooleanExpression()) {
        throwMissingBooleanExpressionError(boolOp);
      }

      next = new TokenNode(*_nIt++);
      if (next->type != TokenType::R_PAREN) {
        throwMissingClosingParenthesisError(next);
      }

      // add R-Paren to tree
      addSiblingAndAdvance(next);

      return true;
    }
    default: {
      // numerical expression & boolean relational expression & numerical
      // expression not handled here anymore, but whats the default case?
      // error? or return false maybe
      revertState(holderNode);
      return false;
    }
    }
    revertState(holderNode);
    return false;
  }
}

bool CSTree::isNumericalExpression() {
  TokenNode *holderNode = _current;
  auto holderIt = _nIt;

  // make flag relevant to deepest call unless set at top level
  _operandFlag = false;

  TokenNode *first = new TokenNode(*_nIt++);
  if (isOperand(first)) {
    // add operand to tree

    addSiblingAndAdvance(first);

    // get and check next
    TokenNode *next = new TokenNode(*_nIt++);

    if (isNumericOperator(next->type)) {
      // add operator to tree

      addSiblingAndAdvance(next);

      next = new TokenNode(*_nIt++);

      if (next->type == TokenType::L_PAREN) {
        // operand + operator + L-Paren + numExp + R-Paren

        // add L-Paren to tree

        addSiblingAndAdvance(next);

        if (!isNumericalExpression()) {
          throwMissingNumericalExpressionError(next);
        }

        next = new TokenNode(*_nIt++);
        if (next->type != TokenType::R_PAREN) {
          throwMissingClosingParenthesisError(next);
        }

        // add R-Paren to tree
        addSiblingAndAdvance(next);
        return true;
      } else {
        // operand + operator + numExp
        // unget last token, to be valid this must be a new NumExp
        TokenType nextType = _nIt->type;
        _nIt--;
        delete next;

        // operand + relationalOp + operand
        if (isRelationalOperator(nextType)) {
          addSiblingAndAdvance(new TokenNode(*_nIt++));
          _operandFlag = true;
          return false;
        }

        if (!isNumericalExpression()) {
          throwMissingNumericalExpressionError(first);
        }
        _operandFlag = false;
        return true;
      }
    } else {
      switch (next->type) {
      case TokenType::ASSIGNMENT_OPERATOR: {
        addSiblingAndAdvance(next);
        return isNumericalExpression();
      }
      case TokenType::PLUS: {
        // i++
        addSiblingAndAdvance(next);
        TokenNode *plus = new TokenNode(*_nIt++);
        if (plus->type != TokenType::PLUS) {
          throwMissingPlusOperatorError(plus);
        }

        addSiblingAndAdvance(plus);
        return true;
      }
      case TokenType::MINUS: {
        // i--
        addSiblingAndAdvance(next);
        TokenNode *minus = new TokenNode(*_nIt++);
        if (minus->type != TokenType::MINUS) {
          throwMissingMinusOperatorError(minus);
        }

        addSiblingAndAdvance(minus);
        return true;
      }

      default: {
        // operand
        _nIt--;
        TokenType type = next->type;

        delete next;
        _operandFlag = true;

        return !isRelationalOperator(type);
      }
      }
    }
  } else {
    switch (first->type) {
    case TokenType::L_PAREN: {
      // L-Paren + operand + R-Paren
      // L-Paren + operand + operator + numExp + R-Paren + operand +
      // operator
      // + numExp L-Paren + operand + operator + numExp + R-Paren

      // all this can be basically reduced to ( numExp ) or ( numExp )
      // operator numExp

      addSiblingAndAdvance(first);

      if (!isNumericalExpression()) {
        if (isRelationalOperator(_nIt->type)) {
          return false;
        }
        throwMissingNumericalExpressionError(first);
      }

      TokenNode *next = new TokenNode(*_nIt++);
      if (next->type != TokenType::R_PAREN) {
        throwMissingClosingParenthesisError(next);
      }

      addSiblingAndAdvance(next);
      return true;
    }
    case TokenType::PLUS: {
      // ++i
      addSiblingAndAdvance(first);
      TokenNode *plus = new TokenNode(*_nIt++);
      if (plus->type != TokenType::PLUS) {
        throwMissingPlusOperatorError(plus);
      }

      addSiblingAndAdvance(plus);

      TokenNode *identifier = new TokenNode(*_nIt++);
      if (!isIdentifier(identifier->type)) {
        throwMissingIdentifierError(identifier);
      }

      addSiblingAndAdvance(identifier);
      return true;
    }
    case TokenType::MINUS: {
      // --i
      addSiblingAndAdvance(first);
      TokenNode *minus = new TokenNode(*_nIt++);
      if (minus->type != TokenType::MINUS) {
        throwMissingMinusOperatorError(minus);
      }
      addSiblingAndAdvance(minus);

      TokenNode *identifier = new TokenNode(*_nIt++);
      if (!isIdentifier(identifier->type)) {
        throwMissingIdentifierError(identifier);
      }
      addSiblingAndAdvance(identifier);
      return true;
    }
    default:
      revertState(holderNode);
      return false;
      // throwSyntaxError(first, "not numExp");
    }
  }
  // for testing
  throw std::runtime_error("not numExp");
  return true;
}

void CSTree::revertState(TokenNode *node) {
  _current = node;
  _nIt--;

  node = node->sibling ? node->sibling : node->child;
  while (node) {
    TokenNode *tempNode = node;
    if (tempNode->sibling) {
      node = tempNode->sibling;
    } else {
      node = tempNode->child;
    }
    delete tempNode;
    _nIt--;
  }
}

// i think this function has a serious flaw with how the exceptions are handled
// does it ever check isNumericalExp if isBool fails, or does it throw ex first
bool CSTree::isExpression() {
  // valid if boolean or numerical expression
  // account for recursion
  return (isBooleanExpression() || isNumericalExpression());
}

bool CSTree::isOperand(TokenNode *token) {
  return token->type == TokenType::IDENTIFIER ||
         token->type == TokenType::INTEGER ||
         token->type == TokenType::FUNCTION ||
         token->type == TokenType::CHAR_LITERAL ||
         (token->type == TokenType::STRING && token->lexeme.length() == 1) ||
         isNumericOperator(token->type);
}

void CSTree::isFunction() {
  TokenNode *next = new TokenNode(*_nIt++);

  // function's datatype

  if (!isDataType(next->type)) {
    throwMissingDatatypeError(next);
  }

  addSiblingAndAdvance(next);

  next = new TokenNode(*_nIt++);

  // identifer for function name

  if (!isIdentifier(next->type)) {
    throwMissingIdentifierError(next);
  } else if (isKeyword(next->type)) {
    throwInvalidFunctionNameError(next);
  }

  addSiblingAndAdvance(next);

  next = new TokenNode(*_nIt++);

  // parenthesis

  if (next->type != TokenType::L_PAREN) {
    throwMissingOpeningParenthesisError(next);
  }
  addSiblingAndAdvance(next);
  _openStack.push(TokenType::L_PAREN);

  // handle inside of ()

  if (!isParameterList()) {
    throwMissingParameterListError(next);
  }

  next = new TokenNode(*_nIt);
  if (next->type != TokenType::R_PAREN) {
    throwMissingClosingParenthesisError(next);
  }
  if (_openStack.top() != TokenType::L_PAREN) {
    throwMissingOpeningParenthesisError(next);
  }

  addSiblingAndAdvance(next);
}

bool CSTree::isParameterList() {
  // datatype identifier
  TokenNode *next = new TokenNode(*_nIt++);
  if (!isIdentifier(next->type)) {
    throwMissingDatatypeError(next);
  }

  addSiblingAndAdvance(next);

  // variable name
  next = new TokenNode(*_nIt++);
  if (!isIdentifier(next->type)) {
    throwMissingIdentifierError(next);
  } else if (isKeyword(next->type)) {
    throwInvalidVariableNameError(next);
  }
  addSiblingAndAdvance(next);

  next = new TokenNode(*_nIt++);

  // array
  if (next->type == TokenType::L_BRACKET) {
    addSiblingAndAdvance(next);

    // array index
    next = new TokenNode(*_nIt++);

    // doesnt currently account for variables be used to access arrays
    if (next->type != TokenType::INTEGER || std::stoi(next->lexeme) < 0) {
      throwArrayNegativeDeclarationError(next);
    }
    addSiblingAndAdvance(next);

    // right bracket
    next = new TokenNode(*_nIt++);
    if (next->type != TokenType::R_BRACKET) {
      // doesnt currently account for numExp or anything other than whole,
      // positive int
      throwMissingClosingBracketError(next);
    }
    addSiblingAndAdvance(next);

    // get next token to allow comma check
    next = new TokenNode(*_nIt++);
  }
  if (next->type == TokenType::COMMA) {
    addSiblingAndAdvance(next);

    _chainCheck = true;
    isParameterList();
    _chainCheck = false;
  } else {
    // ptr? reference? can we just ignore it all for now
    _nIt--; // unget
    delete next;
  }
  return true;
}

void CSTree::isProcedure() {
  // identifer
  TokenNode *next = new TokenNode(*_nIt++);

  // main procedure, treat uniquely
  if (next->type == TokenType::MAIN) {
    addSiblingAndAdvance(next);
    isMain();
    return;
  }

  // else, named procedure
  if (!isIdentifier(next->type)) {
    throwMissingIdentifierError(next);
  } else if (isKeyword(next->type)) {
    throwInvalidProcedureNameError(next);
  }

  addSiblingAndAdvance(next);

  next = new TokenNode(*_nIt++);

  // L-Paren
  if (next->type != TokenType::L_PAREN) {
    throwMissingOpeningParenthesisError(next);
  }
  _openStack.push(TokenType::L_PAREN);
  addSiblingAndAdvance(next);
  next = new TokenNode(*_nIt++);

  // handle inside of (), can be void or paramList
  if (next->type == TokenType::VOID) {
    addSiblingAndAdvance(next);
  } else {
    _nIt--; // delete next? lost mem
    _paramCheck = true;
    if (!isParameterList()) {
      throwSyntaxError(next, "Missing parameter list or void keyword");
    }
    _paramCheck = false;
  }

  next = new TokenNode(*_nIt);

  // R-Paren
  if (next->type != TokenType::R_PAREN) {
    throwMissingClosingParenthesisError(next);
  }
  if (_openStack.top() != TokenType::L_PAREN) {
    throwMissingOpeningParenthesisError(next);
  }

  addSiblingAndAdvance(next);
}

void CSTree::isMain() {
  // L-Paren
  TokenNode *next = new TokenNode(*_nIt++);

  if (next->type != TokenType::L_PAREN) {
    throwMissingOpeningParenthesisError(next);
  }
  _openStack.push(TokenType::L_PAREN);
  addSiblingAndAdvance(next);

  next = new TokenNode(*_nIt++);

  // handle inside of (), must be void
  if (next->type != TokenType::VOID) {
    throwSyntaxError(next, "Main procedure must have no parameters");
  }
  addSiblingAndAdvance(next);

  // R-Paren
  next = new TokenNode(*_nIt);
  if (next->type != TokenType::R_PAREN) {
    throwMissingClosingParenthesisError(next);
  }
  if (_openStack.top() != TokenType::L_PAREN) {
    throwMissingOpeningParenthesisError(next);
  }

  addSiblingAndAdvance(next);
}

void CSTree::isDatatypeSpecifier() {
  // can be:
  // declaration statement
  // dec statement & initializaton statement

  TokenNode *next = new TokenNode(*_nIt++);
  _nIt--; // need node, but intialization list needs one back?

  if (!isIdentifierList()) {
    throwMissingIdentifierListError(next);
  }

  next = new TokenNode(*_nIt++);
  if (next->type != TokenType::SEMICOLON) {
    throwMissingSemicolonError(next);
  }
  // end, add sib, decrement iterator, return

  addSiblingAndAdvance(next);
  _nIt--;
}

bool CSTree::isIdentifierList() {
  TokenNode *next = new TokenNode(*_nIt++);

  // variable name
  if (!isIdentifier(next->type)) {
    throwMissingIdentifierError(next);
  } else if (isKeyword(next->type)) {
    throwInvalidVariableNameError(next);
  }
  addSiblingAndAdvance(next);

  next = new TokenNode(*_nIt++);

  // array
  if (next->type == TokenType::L_BRACKET) {
    _chainCheck = false; // is list, must be well-formed
    addSiblingAndAdvance(next);

    // array index
    next = new TokenNode(*_nIt++);

    // doesnt currently account for variables be used to access arrays
    if (next->type != TokenType::INTEGER || std::stoi(next->lexeme) < 0) {
      throwArrayNegativeDeclarationError(next);
    }
    addSiblingAndAdvance(next);

    // right bracket
    next = new TokenNode(*_nIt++);
    if (next->type != TokenType::R_BRACKET) {
      // doesnt currently account for numExp or anything other than whole,
      // positive int
      throwMissingClosingBracketError(next);
    }
    addSiblingAndAdvance(next);

    // get next token to allow comma check
    next = new TokenNode(*_nIt++);
  }
  if (next->type == TokenType::COMMA) {
    addSiblingAndAdvance(next);

    _chainCheck = true;
    isIdentifierList();
    _chainCheck = false;
  } else {
    // ptr? reference? can we just ignore it all for now
    _nIt--; // unget
    delete next;
  }
  return true;
}
