#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "Tokens.hpp"
#include <cstdint>
#include <string>

class Tokenizer;

class Token {
public:
  explicit Token(std::string data, uint32_t lineNumber, uint32_t charPosition,
                 TokenType type);

  const std::string &getData() const;
  uint32_t getLineNumber() const;
  uint32_t getCharPosition() const;
  TokenType getType() const;

  std::string toString() const;
private:
  void setData(std::string data, uint32_t lineNumber, uint32_t charPosition,
               TokenType type);
  void setLineNumber(uint32_t lineNumber);
  void setCharPosition(uint32_t charPosition);
  void setType(TokenType type);

private:
  Token();

private:
  uint32_t line_number_;
  uint32_t char_pos_;
  TokenType type_;
  std::string data_;

  friend class Tokenizer;
};

#endif // !TOKEN_HPP
#define TOKEN_HPP
