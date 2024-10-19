#ifndef TOKEN_NODE_HPP
#define TOKEN_NODE_HPP

#include "token.hpp"

class TokenNode : public Token {
public:
  TokenNode(const Token &token, TokenNode *sibling = nullptr,
            TokenNode *child = nullptr)
      : Token(token), sibling(sibling), child(child) {}

  TokenNode *sibling;
  TokenNode *child;
};

#endif // TOKEN_NODE_HPP
