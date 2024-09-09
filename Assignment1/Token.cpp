#include "Token.hpp"

Token::Token() = default;

Token::Token(std::string data, TokenType type) {
  setData(data, type);
}

TokenType Token::getType() const {
  return type_;
}

const std::string& Token::getData() const {
  return data_;
}

void Token::setData(std::string data, TokenType type) {
  data_ = std::move(data);
  type_ = type;
}
