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

  if (filename.empty()) {
    std::cout << "usage: program filepath\n";
    exit(1);
  }

  Tokenizer tokenizer(filename);
  Token *token = tokenizer.getNextToken();
  Token *head = token;

  while (token->getType() != TokenType::EndOfFile) {
    if (token->getType() != TokenType::BlockComment &&
        token->getType() != TokenType::SingleLineComment) {
      std::cout << token->getData() << '\n';
    }
    token = tokenizer.getNextToken();
  }
}
