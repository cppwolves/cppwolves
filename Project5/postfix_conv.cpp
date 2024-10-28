/* Numerical Expression Shunting Yard
foreach token in token list
{
  if ((token == INTEGER) || (token == IDENTIFIER) || (token == SINGLE QUOTE) || (token == DOUBLE QUOTE) || (token == STRING))
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
        while (top of stack token != LEFT PARENTHESIS)
        {
          display token at top of stack
          pop stack
        }
        pop stack
      }
      else
      {
        if ((token == PLUS) || (token == MINUS) || (token == ASTERISK) || (token == DIVIDE) || (token == MODULO) || (token == ASSIGNMENT))
        {
          if (stack is empty)
          {
            push token on stack
          }
          else
          {
            if ((token == PLUS) || (token == MINUS))
            {
              finished = FALSE;
              while (not finished)
              {
                if (stack is not empty)
                {
                  if ((top of stack token == PLUS) or (top of stack token == MINUS) or (top of stack token == ASTERISK) or (top of stack token == DIVIDE) or (top of stack token == MODULO))
                  {
                    display token at top of stack
                    pop stack
                  }
                  else
                  {
                    push token on stack
                    finished = TRUE;
                  }
                }
                else
                {
                  push token on stack
                  finished = TRUE;
                }
              }
            }
            else
            {
              if ((token == TOKEN_ASTERISK) || (token == DIVIDE) || (token == TOKEN_MODULO))
              {
                finished = FALSE;
                while (not finished)
                {
                  if (stack is not empty)
                  {
                    if ((top of stack token == ASTERISK) || (top of stack token == DIVIDE) || (top of stack token == TOKEN_MODULO))
                    {
                      display token at top of stack
                      pop stack
                    }
                    else
                    {
                      push token on stack
                      finished = TRUE;
                    }
                  }
                  else
                  {
                    push token on stack
                    finished = TRUE;
                  }
                }
              }
              else
              {
                if (token == ASSIGNMENT OPERATOR)
                {
                  finished = FALSE;
                  while (not finished)
                  {
                    if (stack is not empty)
                    {
                      if ((token == PLUS) || (token == MINUS) || (token == ASTERISK) || (token == DIVIDE) || (token == MODULO))
                      {
                        display token at top of stack
                        pop stack
                      }
                      else
                      {
                        push token on stack
                        finished = TRUE;
                      }
                    }
                    else
                    {
                      push token on stack
                      finished = TRUE;
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

void NumExpPostfixConverter() {
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