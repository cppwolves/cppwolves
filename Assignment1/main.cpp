#include <cassert>
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
/* THIS IS A BLOCK COMMENT */
  {
    // The tokenizer takes the filename to the input file to be scanned as
    // the required arguement. The file is opened automatically.
    Tokenizer tokenizer(filename);

    /* This is a temporary function for testing. This way the ability
      to modify the token can only be handled by Tokenizer
     */
    Token token = Token("", 0, 0, 0, TokenType::None);
    tokenizer.setTokenData(token, "/*", 25, 100, 5,
                           TokenType::BlockCommentStart);
    std::cout << "Updated token: " + token.toString();

    // We can easily reference Token constants by using the Tokens:: namespace
    // identifier
    std::cout << Tokens::BlockCommentStart << "I'm inside the comment block!"
              << Tokens::BlockCommentEnd << Tokens::NewLine;

    std::string token_EOF = Tokens::getTokenName('\0');
    std::cout << "The string \"\\0\" should be represented with the name "
              << "\"EndOfFile\" token: \"\\0\" --> "
              << token_EOF << "\n";
    assert(token_EOF == "EndOfFile");

    std::string null_char = Tokens::getTokenValue(TokenType::EndOfFile);
    std::cout << "We can also input a TokenType, such as TokenType::EndOfFile, "
              << "and get the string \"\\0\" back: TokenType::EndOfFile --> "
              << null_char << "\n\n";
    assert(null_char == "\0");

    std::string token_none = Tokens::getTokenName("asdf");
    std::cout << "If we put in something that is not a token, like the "
              << "string \"asdf\", we should get a string with value \"None\": "
              << "\"asdf\" --> "
              << token_none << "\n";
    assert(token_none == "None");

    std::cout << "Now, let's read the file and print all of the comments:\n\n" << std::flush;

    do {
      token = tokenizer.getNextToken();
      if (token.getType() == TokenType::SingleLineComment || token.getType() == TokenType::BlockComment) {
        std::cout << token.toString() << std::flush;
      }
    } while (token.getType() != TokenType::EndOfFile);

    std::cout << "\nEnd of comments" << '\n' << std::flush;
  }

  {
    Tokenizer tokenizer = Tokenizer(filename);
    std::cout << "Now let's print everything excluding comments:\n\n";
    char c;
    bool escaping = false;
    bool inside_string = false;

    while(c = tokenizer.getNextChar(), c != Tokens::EndOfFile) {
      bool ignore = false;
      if (c == Tokens::DoubleQuote) {
        tokenizer.putBackChar(c);
        std::cout << tokenizer.getNextToken().getData();
        continue;
      }
      if (!inside_string && c == Tokens::LeftParen) {
        char t = tokenizer.getNextChar();
        switch (t) {
        case Tokens::LeftParen:
        case Tokens::Asterisk: {
          tokenizer.putBackChar(t);
          tokenizer.putBackChar(c);
          Token token = tokenizer.getNextToken();
          assert(token.getType() == TokenType::SingleLineComment ||
                 token.getType() == TokenType::BlockComment);
          ignore = true;
        } break;
        default:
          tokenizer.putBackChar(t);
        }
      }
      if (!ignore) {
        std::cout << c;
      }
    }
  }

    // ***************************************************
    /* Joel's ignore-comment work:  */
    // ***************************************************

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

    // ***************************************************
    /* Silvia's ignore-comment work */
    // ***************************************************
    /*
        Adding on to Joel's
        Works for most cases, but not when a fake "comment" is inside a string.
        TestFile #3 shows an example of it failing
        TestFile #6 also fails, needs separate fix
        Does not handle adding back whitespace (" " or "\n"), or writing to output file
    */

    // std::cout << "Starting file read. Ignoring comments, not tokenizing."
    //           << std::endl;

    // std::ifstream file(filename);
    // if (!file.is_open()) {
    //     std::cout << "Failed to open " << filename << "\n";
    //     exit(2);
    // }

    // char c;
    // while (file.get(c) && c != EOF) {
    //     switch (c) {
    //         case '"': {
    //             std::cout << c;
    //             while (file.get(c) && c != '"') {  // inside string
    //                 std::cout << c;
    //             }
    //             std::cout << c;
    //             break;  // ADD: check for end of file;
    //         }
    //         case '/': {
    //             if (file.get(c)) {
    //                 if (c == '/') {  // line comment
    //                     while (file.get(c) && c != '\n') {
    //                         // ADD: handle whitespace
    //                         continue;
    //                     }
    //                 } else if (c == '*') {  // block comment
    //                     bool commentClosed = false;
    //                     while (file.get(c)) {
    //                         // ADD: handle whitespace
    //                         if (c == '*') {
    //                             if (file.get(c) && c == '/') {
    //                                 commentClosed = true;
    //                                 break;
    //                             } else {
    //                                 file.putback(c);
    //                             }
    //                         }
    //                     }
    //                     if (!commentClosed) {
    //                         std::cout << "ERROR: Exiting, comment unterminated." << std::endl;
    //                         break;
    //                     }
    //                     continue;
    //                 }
    //             }
    //             default:
    //                 std::cout << c;
    //         }
    //             // case '*':{  //terminated comment that wasn't started
    //             // ADD: check for '/'
    //             // }
    //     }
    // }
}
