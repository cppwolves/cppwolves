#include "Token.hpp"
#include "Tokens.hpp"
#include <cstdint>
#include <string>

Token::Token() = default;

Token::Token(std::string data, uint32_t lineNumber, uint32_t charColumn,
             uint32_t charIndex, TokenType type) {
  setData(data, lineNumber, charColumn, charIndex, type);
}

Token::Token(char data, uint32_t lineNumber, uint32_t charColumn,
             uint32_t charIndex, TokenType type) {
  setData(std::string(&data), lineNumber, charColumn, charIndex, type);
}

const std::string &Token::getData() const { return data_; }

uint32_t Token::getLineNumber() const { return line_number_; }

uint32_t Token::getCharIndex() const { return char_index_; }

uint32_t Token::getCharColumn() const { return char_col_; }

TokenType Token::getType() const { return type_; }

std::string Token::toString() const {
  return "line: " + std::to_string(line_number_) +
         ", column: " + std::to_string(char_col_) +
         ", index: " + std::to_string(char_index_) + ", type: (" +
         std::to_string(int(type_)) + ") " + Tokens::getTokenName(type_) +
         ", data: \"" + data_ + "\"\n";
}

void Token::setLineNumber(uint32_t lineNumber) { line_number_ = lineNumber; }

void Token::setCharIndex(uint32_t charPosition) { char_index_ = charPosition; }

void Token::setCharColumn(uint32_t charColumn) { char_col_ = charColumn; }

void Token::setData(std::string data, uint32_t lineNumber, uint32_t charColumn,
                    uint32_t charIndex, TokenType type) {
  data_ = std::move(data);
  line_number_ = lineNumber;
  char_index_ = charIndex;
  char_col_ = charColumn;
  type_ = type;
}
void Token::setType(TokenType type) { type_ = type; }
