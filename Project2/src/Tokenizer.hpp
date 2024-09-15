#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <climits>
#include <cstdint>
#include <fstream>
#include <string>

#include "Token.hpp"
#include "Tokens.hpp"

class Token;

class Tokenizer {
public:
  Tokenizer();
  explicit Tokenizer(std::string filename);
  ~Tokenizer();

  char getNextChar();
  Token *getNextToken();
  Token *getNextCharToken();

  void putBackChar(char c);
  void putBackToken(Token *token);

  void setTokenData(Token *token, std::string data, uint32_t lineNumber,
                    uint32_t charColumn, uint32_t charIndex, TokenType type);

private:
  void parseSingleLineComment(Token *token);
  void parseBlockComment(Token *token);
  void parseString(Token *token);

  void printErrorAndExit(std::string &&message);

private:
  uint32_t row_{};

  int32_t last_row_{};
  int32_t index_{};
  int32_t last_index_{};
  int32_t col_{};
  int32_t last_col_{};

  std::string filename_{};
  std::fstream fstream_{};
};
#endif
