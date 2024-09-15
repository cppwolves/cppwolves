#include <cstdio>
#include <iostream>
#include <string>

#include "Tokenizer.hpp"
#include "Tokens.hpp"

int main(int argc, char *argv[]) {
  std::string filename{};
  if (argc > 1) {
    filename = argv[1];
  }

  if (filename.size() == 0) {
    std::cout << "usage: program filepath\n";
    exit(1);
  }

  Tokenizer tokenizer(filename);
  char c = tokenizer.getNextChar();
  while (c != Tokens::EndOfFile) {
    switch (c) {
    case Tokens::DoubleQuote: {
      tokenizer.putBackChar(c);
      std::cout << tokenizer.getNextToken()->getData();
    } break;
    case Tokens::LeftSlash: {
      char t = tokenizer.getNextChar();
      switch (t) {
      case Tokens::LeftSlash:
      case Tokens::Asterisk: {
        tokenizer.putBackChar(t);
        tokenizer.putBackChar(c);
        Token *token = tokenizer.getNextToken();
      } break;
      default:
        tokenizer.putBackChar(t);
        std::cout << c;
      }
    } break;
    case Tokens::Asterisk:
      tokenizer.putBackChar(c);
      // This will throw an error since this asterisk should not be here.
      tokenizer.getNextToken();
    default:
      std::cout << c;
    }
    c = tokenizer.getNextChar();
  }
}
