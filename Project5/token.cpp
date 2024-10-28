// token.cpp
#include "token.hpp"
#include "token_enum.hpp"
#include <string>

Token::Token(TokenType type, const std::string &lexeme, int lineNumber)
    : type(type), lexeme(lexeme), lineNumber(lineNumber) {}

Token::Token(const Token &token)
    : type(token.type), lexeme(token.lexeme), lineNumber(token.lineNumber) {}

std::string Token::getTypeName() const {
  return std::string(typeToCString(type));
}
