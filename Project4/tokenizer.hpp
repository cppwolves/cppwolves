// tokenizer.hpp
#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include "token.hpp"
#include <fstream>
#include <queue>
#include <string>
#include <vector>

class Tokenizer {
public:
  Tokenizer(const std::string &filename);
  std::vector<Token> tokenize();
  std::string errorMessage;
  std::queue<Token> tokenQueue;

private:
  std::vector<Token> tokens;
  std::ifstream inputFile;
  int lineNumber;
  char currentChar;
  bool endOfFile;

  // Helper methods
  void advance();
  void skipWhitespace();
  void skipComment();

  Token getNextToken();
  Token identifierOrKeyword();
  Token number();
  Token stringLiteral();
  Token charLiteral();
  Token operatorOrDelimiter();

  bool isKeyword(const std::string &str);
  bool isLetter(char ch);
  bool isDigit(char ch);
  bool isHexDigit(char ch);
  bool isOperatorChar(char ch);
};

#endif // TOKENIZER_HPP
