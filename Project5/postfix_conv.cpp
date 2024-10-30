#include <iostream>
#include <stack>

#include "token_enum.hpp"
#include "token_node.hpp"

/* Numerical Expression Shunting Yard */

void displayToken(TokenNode* currToken) {
    std::cout << currToken->lexeme << " ";
}

void NumExpPostfixConverter(TokenNode* currToken, TokenNode* endToken) {
    std::stack<TokenNode*> _holdStack;
    bool _finished = false;  // looping flag
    TokenNode* topToken = nullptr;

    // 'display' token meaning add to ASTree
    // function could return a pointer to a chain of AST nodes, handled upon return to caller
    // currently: printing to std::cout

    // movement loop
    // endNode must be a sibling, and have be set before function call
    for (; currToken != endToken; currToken = currToken->sibling) {
        switch (currToken->type) {
            case TokenType::INTEGER:
            case TokenType::IDENTIFIER:
            case TokenType::SINGLE_QUOTE:
            case TokenType::DOUBLE_QUOTE:
            case TokenType::STRING: {
                displayToken(currToken);
                break;
            }
            case TokenType::L_PAREN: {
                _holdStack.push(currToken);
                break;
            }
            case TokenType::R_PAREN: {
                topToken = _holdStack.top();
                while (topToken->type != TokenType::L_PAREN) {
                    displayToken(topToken);
                    _holdStack.pop();
                    if (!_holdStack.empty()) {
                        topToken = _holdStack.top();
                    }
                }
                _holdStack.pop();
                break;
            }
            case TokenType::PLUS:
            case TokenType::MINUS:
            case TokenType::ASTERISK:
            case TokenType::DIVIDE:
            case TokenType::MODULO:
            case TokenType::ASSIGNMENT_OPERATOR: {
                if (_holdStack.empty()) {
                    _holdStack.push(currToken);
                } else {
                    switch (currToken->type) {
                        case TokenType::PLUS:
                        case TokenType::MINUS: {
                            _finished = false;
                            while (!_finished) {
                                if (!_holdStack.empty()) {
                                    topToken = _holdStack.top();
                                    if ((topToken->type == TokenType::PLUS) ||
                                        (topToken->type == TokenType::MINUS) ||
                                        (topToken->type == TokenType::ASTERISK) ||
                                        (topToken->type == TokenType::DIVIDE) ||
                                        (topToken->type == TokenType::MODULO)) {
                                        // display token at top of stack
                                        displayToken(topToken);
                                        _holdStack.pop();
                                    } else {
                                        // push token on stack
                                        _holdStack.push(currToken);
                                        _finished = true;
                                    }
                                } else {
                                    // push token on stack
                                    _holdStack.push(currToken);
                                    _finished = true;
                                }
                            }
                            break;
                        }
                        case TokenType::ASTERISK:
                        case TokenType::DIVIDE:
                        case TokenType::MODULO: {
                            _finished = false;
                            while (!_finished) {
                                if (!_holdStack.empty()) {
                                    topToken = _holdStack.top();
                                    if ((topToken->type == TokenType::ASTERISK) ||
                                        (topToken->type == TokenType::DIVIDE) ||
                                        (topToken->type == TokenType::MODULO)) {
                                        // display token at top of stack
                                        // pop stack

                                        displayToken(topToken);
                                        _holdStack.pop();
                                    } else {
                                        // push token on stack
                                        _holdStack.push(currToken);
                                        _finished = true;
                                    }
                                } else {
                                    // push token on stack
                                    _holdStack.push(currToken);
                                    _finished = true;
                                }
                            }
                            break;
                        }
                        case TokenType::ASSIGNMENT_OPERATOR: {
                            _finished = false;
                            while (!_finished) {
                                if (!_holdStack.empty()) {
                                    topToken = _holdStack.top();
                                    if ((currToken->type == TokenType::PLUS) ||
                                        (currToken->type == TokenType::MINUS) ||
                                        (currToken->type == TokenType::ASTERISK) ||
                                        (currToken->type == TokenType::DIVIDE) ||
                                        (currToken->type == TokenType::MODULO)) {
                                        // display token at top of stack
                                        // pop stack
                                        displayToken(topToken);
                                        _holdStack.pop();
                                        topToken = _holdStack.top();

                                    } else {
                                        // push token on stack
                                        _holdStack.push(currToken);
                                        _finished = true;
                                    }
                                } else {
                                    // push token on stack
                                    _holdStack.push(currToken);
                                    _finished = true;
                                }
                            }
                            break;
                        }
                    }
                }
                break;
            }
            default: {
                // error out?
                // should never reach this point, all cst rows should be correct
            }
        }
    }
    while (!_holdStack.empty()) {
        // display token at top of stack
        // pop stack
        topToken = _holdStack.top();
        displayToken(topToken);
        _holdStack.pop();
    }
    std::cout << std::endl;
}

/* Boolean Exp Shunting Yard
foreach token in token list
{
  if ((token == INTEGER) || (token == IDENTIFIER) || (token == SINGLE QUOTE) || (token == DOUBLE QUOTE) ||
      (token == STRING) || (token == LEFT BRACKET) || (token == RIGHT BRACKET))
  {
    display token
  }
  else
  {
    if (token == LEFT PARENTHESIS)
    {
      push token on stack
    }
    else
    {
      if (token == RIGHT PARENTHESIS)
      {
        finished = FALSE
        while (not finished)
        {
          if (top of stack token == LEFT PARENTHESIS)
          {
            pop stack
            finished = TRUE
          }
          else
          {
            display token at top of stack
            pop stack
          }
        }
      }
      else
      {
        if ((token == BOOLEAN_EQUAL)      || (token == NOT_EQUAL)             || (token == LESS_THAN)   || (token == GREATER_THAN) ||
            (token == LESS_THAN_OR_EQUAL) || (token == GREATER_THAN_OR_EQUAL) || (token == BOOLEAN_AND) || (token == BOOLEAN_OR)   ||
            (token == BOOLEAN_NOT)        || (token == PLUS)                  || (token == MINUS)       || (token == ASTERISK)     ||
            (token == DIVIDE)             || (token == MODULO))
        {
          if (stack is empty)
          {
            push token on stack
          }
          else
          {
            if (top of stack token == LEFT PARENTHESIS)
            {
              push token on stack
            }
            else
            {
              if (token == BOOLEAN_NOT)
              {
                finished = FALSE
                while (not finished)
                {
                  if (stack is not empty)
                  {
                    if (top of stack token == BOOLEAN NOT)
                    {
                      display token at top of stack
                      pop stack
                    }
                    else
                    {
                      push token on stack
                      finished = TRUE
                    }
                  }
                  else
                  {
                    push token on stack
                    finished = TRUE
                  }
                }
              }
              else
              {
                if ((token == ASTERISK) || (token == TOKEN_DIVIDE) || (token == TOKEN_MODULO))
                {
                  finished = FALSE
                  while (not finished)
                  {
                    if (stack is not empty)
                    {
                      if ((top of stack token == BOOLEAN_NOT)  || (top of stack token == TOKEN_ASTERISK) || (top of stack token == TOKEN_DIVIDE) || (stack->token->token_type == TOKEN_MODULO))
                      {
                        display token at top of stack
                        pop stack
                      }
                      else
                      {
                        push token on stack
                        finished = TRUE
                      }
                    }
                    else
                    {
                      push token on stack
                      finished = TRUE
                    }
                  }
                }
                else
                {
                  if ((token == PLUS) || (token == MINUS))
                  {
                    finished = FALSE
                    while (not finished)
                    {
                      if (stack is not empty)
                      {
                        if ((top of stack token == BOOLEAN_NOT) || (top of stack token == ASTERISK) ||
                            (top of stack token == DIVIDE)      || (top of stack token == MODULO)   ||
                            (top of stack token == PLUS)        || (top of stack token == MINUS))
                        {
                          display token at top of stack
                          pop stack
                        }
                        else
                        {
                          push token on stack
                          finished = TRUE
                        }
                      }
                      else
                      {
                        push token on stack
                        finished = TRUE
                      }
                    }
                  }
                  else
                  {
                    if ((token == BOOLEAN_EQUAL)      || (token == NOT_EQUAL) || (token == LESS_THAN) || (token == GREATER_THAN) ||
                        (token == LESS_THAN_OR_EQUAL) || (token == GREATER_THAN_OR_EQUAL))
                    {
                      finished = FALSE
                      while (not finished)
                      {
                        if (stack is not empty)
                        {
                          if ((top of stack token == BOOLEAN_NOT)        || (top of stack token == ASTERISK)              ||
                              (top of stack token == DIVIDE)             || (top of stack token == MODULO)                ||
                              (top of stack token == PLUS)               || (top of stack token == MINUS)                 ||
                              (top of stack token == BOOLEAN_EQUAL)      || (top of stack token == NOT_EQUAL)             ||
                              (top of stack token == LESS_THAN)          || (top of stack token == GREATER_THAN)          ||
                              (top of stack token == LESS_THAN_OR_EQUAL) || (top of stack token == GREATER_THAN_OR_EQUAL) ||
                              (top of stack token == TOKEN_BOOLEAN_NOT))
                          {
                            display token at top of stack
                            pop stack
                          }
                          else
                          {
                            push token on stack
                            finished = TRUE
                          }
                        }
                        else
                        {
                          push token on stack
                          finished = TRUE
                        }
                      }
                    }
                    else
                    {
                      if (token == BOOLEAN_AND)
                      {
                        finished = FALSE
                        while (not finished)
                        {
                          if (stack is not empty)
                          {
                            if ((top of stack token == BOOLEAN_NOT)        || (top of stack token == ASTERISK)              ||
                                (top of stack token == DIVIDE)             || (top of stack token == MODULO)                ||
                                (top of stack token == PLUS)               || (top of stack token == MINUS)                 ||
                                (top of stack token == BOOLEAN_EQUAL)      || (top of stack token == NOT_EQUAL)             ||
                                (top of stack token == LESS_THAN)          || (top of stack token == GREATER_THAN)          ||
                                (top of stack token == LESS_THAN_OR_EQUAL) || (top of stack token == GREATER_THAN_OR_EQUAL) ||
                                (top of stack token == BOOLEAN_AND)        || (top of stack token == BOOLEAN_NOT))
                            {
                              display token at top of stack
                              pop stack
                            }
                            else
                            {
                              push token on stack
                              finished = TRUE
                            }
                          }
                          else
                          {
                            push token on stack
                            finished = TRUE
                          }
                        }
                      }
                      else
                      {
                        if (token == BOOLEAN_OR)
                        {
                          finished = FALSE
                          while (not finished)
                          {
                            if (stack is not empty)
                            {
                              if ((top of stack token == BOOLEAN_NOT)        || (top of stack token == ASTERISK)              ||
                                  (top of stack token == DIVIDE)             || (top of stack token == MODULO)                ||
                                  (top of stack token == PLUS)               || (top of stack token == MINUS)                 ||
                                  (top of stack token == BOOLEAN_EQUAL)      || (top of stack token == NOT_EQUAL)             ||
                                  (top of stack token == LESS_THAN)          || (top of stack token == GREATER_THAN)          ||
                                  (top of stack token == LESS_THAN_OR_EQUAL) || (top of stack token == GREATER_THAN_OR_EQUAL) ||
                                  (top of stack token == BOOLEAN_AND)        || (top of stack token == BOOLEAN_OR)            ||
                                  (top of stack token == BOOLEAN_NOT))
                              {
                                display token at top of stack
                                pop stack
                              }
                              else
                              {
                                push token on stack
                                finished = TRUE
                              }
                            }
                            else
                            {
                              push token on stack
                              finished = TRUE
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}
while (stack is not empty)
{
  display token at top of stack
  pop stack
}
*/

void BoolExpPostfixConverter() {
}

// non switch case version (his)
void ogExp(TokenNode* currToken, TokenNode* endToken) {
    std::stack<TokenNode*> _holdStack;
    bool _finished = false;  // looping flag
    TokenNode* topToken = nullptr;

    // 'display' token meaning add to ASTree
    // function could return a pointer to a chain of AST nodes, handled upon return to caller
    // currently: printing to std::cout

    // movement loop
    // endNode must be a sibling, and have be set before function call
    for (; currToken != endToken; currToken = currToken->sibling) {
        if ((currToken->type == TokenType::INTEGER) ||
            (currToken->type == TokenType::IDENTIFIER) ||
            (currToken->type == TokenType::SINGLE_QUOTE) ||
            (currToken->type == TokenType::DOUBLE_QUOTE) ||
            (currToken->type == TokenType::STRING)) {
            // display token
            displayToken(currToken);
        } else {
            if (currToken->type == TokenType::L_PAREN) {
                _holdStack.push(currToken);

            } else {
                if (currToken->type == TokenType::R_PAREN) {
                    topToken = _holdStack.top();
                    while (topToken->type != TokenType::L_PAREN) {
                        // display token at top of stack
                        // pop stack
                        displayToken(topToken);
                        _holdStack.pop();
                        if (!_holdStack.empty()) {
                            topToken = _holdStack.top();
                        }
                    }
                    _holdStack.pop();
                } else {
                    if ((currToken->type == TokenType::PLUS) ||
                        (currToken->type == TokenType::MINUS) ||
                        (currToken->type == TokenType::ASTERISK) ||
                        (currToken->type == TokenType::DIVIDE) ||
                        (currToken->type == TokenType::MODULO) ||
                        (currToken->type == TokenType::ASSIGNMENT_OPERATOR)) {
                        if (_holdStack.empty()) {
                            _holdStack.push(currToken);
                        } else {
                            if ((currToken->type == TokenType::PLUS) || (currToken->type == TokenType::MINUS)) {
                                _finished = false;
                                while (!_finished) {
                                    if (!_holdStack.empty()) {
                                        topToken = _holdStack.top();
                                        if ((topToken->type == TokenType::PLUS) ||
                                            (topToken->type == TokenType::MINUS) ||
                                            (topToken->type == TokenType::ASTERISK) ||
                                            (topToken->type == TokenType::DIVIDE) ||
                                            (topToken->type == TokenType::MODULO)) {
                                            // display token at top of stack
                                            displayToken(topToken);
                                            _holdStack.pop();
                                        } else {
                                            // push token on stack
                                            _holdStack.push(currToken);
                                            _finished = true;
                                        }
                                    } else {
                                        // push token on stack
                                        _holdStack.push(currToken);
                                        _finished = true;
                                    }
                                }
                            } else {
                                if ((currToken->type == TokenType::ASTERISK) ||
                                    (currToken->type == TokenType::DIVIDE) ||
                                    (currToken->type == TokenType::MODULO)) {
                                    _finished = false;
                                    while (!_finished) {
                                        if (!_holdStack.empty()) {
                                            topToken = _holdStack.top();
                                            if ((topToken->type == TokenType::ASTERISK) ||
                                                (topToken->type == TokenType::DIVIDE) ||
                                                (topToken->type == TokenType::MODULO)) {
                                                // display token at top of stack
                                                // pop stack

                                                displayToken(topToken);
                                                _holdStack.pop();
                                            } else {
                                                // push token on stack
                                                _holdStack.push(currToken);
                                                _finished = true;
                                            }
                                        } else {
                                            // push token on stack
                                            _holdStack.push(currToken);
                                            _finished = true;
                                        }
                                    }
                                } else {
                                    if (currToken->type == TokenType::ASSIGNMENT_OPERATOR) {
                                        _finished = false;
                                        while (!_finished) {
                                            if (!_holdStack.empty()) {
                                                topToken = _holdStack.top();
                                                if ((currToken->type == TokenType::PLUS) ||
                                                    (currToken->type == TokenType::MINUS) ||
                                                    (currToken->type == TokenType::ASTERISK) ||
                                                    (currToken->type == TokenType::DIVIDE) ||
                                                    (currToken->type == TokenType::MODULO)) {
                                                    // display token at top of stack
                                                    // pop stack
                                                    displayToken(topToken);
                                                    _holdStack.pop();
                                                    topToken = _holdStack.top();

                                                } else {
                                                    // push token on stack
                                                    _holdStack.push(currToken);
                                                    _finished = true;
                                                }
                                            } else {
                                                // push token on stack
                                                _holdStack.push(currToken);
                                                _finished = true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    while (!_holdStack.empty()) {
        // display token at top of stack
        // pop stack
        topToken = _holdStack.top();
        displayToken(topToken);
        _holdStack.pop();
    }
}
