#include "Token.hpp"
#include "Tokens.hpp"
#include <cstdint>
#include <string>

Token::Token() = default;

Token::Token(std::string data, uint32_t lineNumber, uint32_t charPosition,
             TokenType type) {
  setData(data, lineNumber, charPosition, type);
}

const std::string &Token::getData() const { return data_; }

uint32_t Token::getLineNumber() const { return line_number_; }

uint32_t Token::getCharPosition() const { return char_pos_; }

TokenType Token::getType() const { return type_; }

std::string Token::toString() const {
  return "Line: " + std::to_string(line_number_) +
         ", Char position: " + std::to_string(char_pos_) + ", Type: (" +
         std::to_string(int(type_)) + ") " + Tokens::getTokenValue(type_) +
         ", Data: \"" + data_ + "\"\n";
}

void Token::setLineNumber(uint32_t lineNumber) { line_number_ = lineNumber; }

void Token::setCharPosition(uint32_t charPosition) { char_pos_ = charPosition; }

void Token::setData(std::string data, uint32_t lineNumber,
                    uint32_t charPosition, TokenType type) {
  data_ = std::move(data);
  line_number_ = lineNumber;
  char_pos_ = charPosition;
  type_ = type;
}
void Token::setType(TokenType type) { type_ = type; }
