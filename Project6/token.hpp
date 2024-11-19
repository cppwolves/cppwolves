// token.hpp
#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "token_enum.hpp"
#include <string>

class Token {
public:
  Token(TokenType type, const std::string &lexeme, int lineNumber);
  Token(const Token &token);

  std::string getTypeName() const;

  TokenType type;
  std::string lexeme;
  int lineNumber;
};

#endif // TOKEN_HPP
