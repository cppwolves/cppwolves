#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include "Token.hpp"
#include "Tokens.hpp"

class Token;

class Tokenizer {
public:
  Tokenizer();
  explicit Tokenizer(std::string filename);

  void setTokenData(Token& token, std::string data, uint32_t lineNumber,
                     uint32_t charPosition, TokenType type);
  Token getNextToken();
  void putBackToken(Token token);

private:
  uint32_t line_number_{};
  uint32_t char_pos_{};
  std::string filename_{};
  std::fstream fstream_{};
  std::vector<Token> token_stack_{};
};
#endif
