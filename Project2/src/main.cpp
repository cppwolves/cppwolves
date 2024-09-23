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

  // Output issues: 
  // - Not tokenizing/typing: semicolon, identifier, assignment operator, integer
  // - String tokens are being prematurally terminated
  // - His output doesn't consider '\n' a token, ours does

  std::cout << "Token list:\n" << std::endl;

  while (token->getType() != TokenType::EndOfFile) {
    if (token->getType() != TokenType::BlockComment &&
        token->getType() != TokenType::SingleLineComment) {
      std::cout << "Token type: " << Tokens::getTokenName(token->getType()) << '\n';
      std::cout << "Token:\t" << token->getData() << "\n\n";
    }
    token = tokenizer.getNextToken();
  }
}

