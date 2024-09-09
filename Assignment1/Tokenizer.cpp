#include "Tokenizer.hpp"
#include "Tokens.hpp"
#include <cassert>
#include <cstdint>

Tokenizer::Tokenizer()
    : line_number_(0), char_pos_(0), fstream_(nullptr), token_stack_({}) {}

Tokenizer::Tokenizer(std::string filename)
    : line_number_(0), char_pos_(0), filename_(std::move(filename)),
      fstream_({}), token_stack_({}) {
  fstream_.open(filename_);
  assert(fstream_.is_open() && "Could not open file");
}

void Tokenizer::setTokenData(Token& token, std::string data, uint32_t lineNumber, uint32_t charPosition, TokenType type){
  token.setData(data, lineNumber, charPosition, type);
}

Token Tokenizer::getNextToken() {
  if (!token_stack_.empty()) {
    Token token = token_stack_.back();
    token_stack_.pop_back();
    return token;
  }
  std::string nodata = "";
  return Token(nodata, 0, 0, TokenType::None);
}

void Tokenizer::putBackToken(Token token) { token_stack_.push_back(token); }
