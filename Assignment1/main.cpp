#include <cstdio>
#include <cassert>
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

  // The tokenizer takes the filename to the input file to be scanned as
  // the required arguement. The file is opened automatically.
  Tokenizer tokenizer(filename);

  // As of now, this function returns a "Null" token
  Token null_token = tokenizer.getNextToken();
  std::cout << "Initial token: " + null_token.toString();

  // This is a temporary function for testing. This way the ability
  // to modify the token can only be handled by Tokenizer
  tokenizer.setTokenData(null_token, "/*", 100, 5,
                         TokenType::BlockCommentStart);
  std::cout << "Updated token: " + null_token.toString();

  // We can easily reference Token constants by using the Tokens:: namespace
  // identifier
  std::cout << Tokens::BlockCommentStart << "I'm inside the comment block!"
            << Tokens::BlockCommentEnd << Tokens::NewLine;

  std::string token_EOF = Tokens::getTokenName("\0");
  std::cout << "The string \"\\0\" should be represented with the name "
               "\"EndOfFile\" token: \"\\0\" --> "
            << token_EOF << '\n';
  assert(token_EOF == "EndOfFile");

  std::string null_char =Tokens::getTokenValue(TokenType::EndOfFile); 
  std::cout << "We can also input a TokenType, such as TokenType::EndOfFile, and get the "
               "string \"\\0\" back: TokenType::EndOfFile --> "
            << null_char << '\n';
  assert(null_char == "\0");

  std::string token_none = Tokens::getTokenName("asdf");
  std::cout << "If we put in something that is not a token, like the "
               "string \"asdf\", we should get a string with value \"None\": "
               "\"asdf\" --> "
            << token_none << '\n';
  assert(token_none == "None");

  // std::ifstream file(filename);
  // if (!file.is_open()) {
  //   std::cout << "Failed to open " << filename << "\n";
  //   exit(2);
  // }
  //
  // char c;
  // while (file.get(c) && c != EOF) {
  //   switch (c) {
  //   case '/': {
  //     if (file.get(c) && c == '/') {
  //       while (file.get(c) && c != '\n') {
  //         continue;
  //         // std::cout << c;
  //       }
  //       // std::cout << c;
  //     } else {
  //       file.putback(c);
  //     }
  //     break;
  //   }
  //   default:
  //     std::cout << c;
  //   }
  // }
}
