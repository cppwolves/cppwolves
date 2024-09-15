#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "Tokens.hpp"
#include <cstdint>
#include <string>

class Tokenizer;

class Token {
public:
  explicit Token(std::string data, uint32_t lineNumber, uint32_t charColumn,
                 uint32_t charPosition, TokenType type);
  explicit Token(char &data, uint32_t lineNumber, uint32_t charColumn,
                 uint32_t charPosition, TokenType type);
  ~Token() = default;

  const std::string &getData() const;
  uint32_t getLineNumber() const;
  uint32_t getCharIndex() const;
  uint32_t getCharColumn() const;
  TokenType getType() const;

  Token *getRightSibling();
  Token *getLeftChild();

  void setRightSibling(Token *token);
  void setLeftChild(Token *token);

  std::string toString() const;

private:
  void setData(std::string data, uint32_t lineNumber, uint32_t charColumn,
               uint32_t charPosition, TokenType type);
  void setLineNumber(uint32_t lineNumber);
  void setCharIndex(uint32_t charIndex);
  void setCharColumn(uint32_t charColumn);
  void setType(TokenType type);

private:
  Token();

private:
  uint32_t line_number_;
  uint32_t char_index_;
  uint32_t char_col_;
  TokenType type_;
  std::string data_;

  Token *right_sibling_{nullptr};
  Token *left_child_{nullptr};

  friend class Tokenizer;
};

#endif // !TOKEN_HPP
#define TOKEN_HPP
