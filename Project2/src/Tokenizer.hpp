#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <cstdint>
#include <fstream>
#include <string>

#include "Token.hpp"
#include "Tokens.hpp"

class Token;

class Tokenizer {
  enum Flag {
    NONE,
    READING_STRING,
    READ_COMMENT,
  };

  enum State {
    NOT_INITIALIZED,
    END_OF_FILE,
    READ_START,
    READ_END,
    READ_LSLASH,
    READ_RSLASH,
    READ_NAME,
    READ_CHAR,
    READ_DIGIT,
    READ_NUMBER,
    READ_SINGLE_LINE_COMMENT,
    READ_BLOCK_COMMENT,
  };

  enum ExitCode {
    EXIT_SYNTAX_ERROR,
    EXIT_FILE_READ_ERROR,
    EXIT_FILENAME_ERROR,
    EXIT_FILE_OPEN_ERROR,
    EXIT_UNTERMINATED_COMMENT,
    EXIT_UNTERMINATED_STRING,
    EXIT_MISSING_BLOCK_COMMENT_START,
  };

public:
  Tokenizer();
  explicit Tokenizer(std::string filename);
  ~Tokenizer();

  Token *getNextToken();

  void setTokenData(Token *token, const std::string &data, uint32_t lineNumber,
                    uint32_t charColumn, uint32_t charIndex, TokenType type);

private:
  char getNextChar();
  void putBackChar(char c);
  char peekNextChar();

  void parseSingleLineComment(Token *token);
  void parseBlockComment(Token *token);
  void parseString(Token *token);

  void printErrorAndExit(ExitCode code, const Token *token) const;

  // States
  Token *stateReadStart();
  void stateReadComment(Token *token, char &c);
  void stateReadChar(Token *token, char &c);
  void stateReadDigit(Token *token, const char &c);

private:
  int32_t row_{};
  int32_t last_row_{};
  int32_t index_{};
  int32_t last_index_{};
  int32_t col_{};
  int32_t last_col_{};

  std::string filename_{};
  std::ifstream ifstream_{};

  State state_{};
  Flag flag_{NONE};
};
#endif
