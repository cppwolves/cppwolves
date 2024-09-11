#include "Tokenizer.hpp"
#include "Tokens.hpp"
#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>

Tokenizer::Tokenizer()
    : row_(0), index_(-1), col_(-1), fstream_(nullptr), token_stack_({}) {}

Tokenizer::Tokenizer(std::string filename)
    : row_(0), index_(-1), col_(-1), filename_(std::move(filename)),
      fstream_({}), token_stack_({}) {
  fstream_.open(filename_);
  assert(fstream_.is_open() && "Could not open file");
  fstream_.seekg(0);
}

Tokenizer::~Tokenizer() { fstream_.close(); }

void Tokenizer::setTokenData(Token &token, std::string data,
                             uint32_t lineNumber, uint32_t charColumn,
                             uint32_t charIndex, TokenType type) {
  token.setData(data, lineNumber, charColumn, charIndex, type);
}

Token Tokenizer::getNextToken() {
  if (!token_stack_.empty()) {
    Token token = token_stack_.back();
    token_stack_.pop_back();

    fstream_.seekg(token.char_index_);
    row_ = token.line_number_;
    last_row_ = row_ - 1 ;
    index_ = token.char_index_;
    last_index_ = index_ - 1;
    col_ = token.char_col_;
    last_col_ = col_ - 1;

    return token;
  }

  Token token = getNextCharToken();
  switch (token.type_) {
  case TokenType::LeftParen: {
    char c = getNextChar();
    switch (c) {
    case Tokens::LeftParen:
      putBackChar(c);
      parseSingleLineComment(token);
      break;
    case Tokens::Asterisk:
      putBackChar(c);
      parseBlockComment(token);
      break;
    default:
      if (c != Tokens::EndOfFile) {
        putBackChar(c);
      }
      break;
    }
    break;
  }
  case TokenType::DoubleQuote:
    parseString(token);
    break;
  default:
    break;
  }

  // std::cout << "Leaving getNextToken: " + token.toString();

  return token;
}

void Tokenizer::putBackToken(Token token) { token_stack_.push_back(token); }

void Tokenizer::putBackChar(char c) {
  assert(index_ >= 0 &&
         "Cannot put back char -- already at beginning of stream");
  assert(fstream_.putback(c) && "Putback failed");

  if (c == Tokens::NewLine) {
    row_ = last_row_;
    last_row_ = row_ -1;
  }
  col_ = last_col_--;
  index_--;
  last_index_--;
}

char Tokenizer::getNextChar() {
  char c;
  if (fstream_.eof()) {
    c = Tokens::EndOfFile;
  } else {
    fstream_.get(c);
    if (c != Tokens::EndOfFile) {
      if (c == Tokens::NewLine) {
        last_row_ = row_++;
        last_col_ = col_;
        col_ = 0;
    } else {
        if (row_ == 0) {
          row_ = 1;
        }
        last_col_ = col_++;
      }
      last_index_ = index_++;
    }
  }
  return c;
}

Token Tokenizer::getNextCharToken() {
  // std::cout << "Entering getNextCharToken\n";
  char c;
  TokenType token_type = TokenType::None;

  if (fstream_.eof()) {
    token_type = TokenType::EndOfFile;
  } else {
    do {
      c = getNextChar();
      if (TokenType type = Tokens::getTokenType(c); type != TokenType::None) {
        token_type = type;
        break;
      }
    } while (fstream_.good() && c != Tokens::EndOfFile);
  }

  Token token = Token(c, row_, col_, index_, token_type);
  // std::cout << "Leaving getNextCharToken: " + token.toString();
  return token;
}

void Tokenizer::parseSingleLineComment(Token &tokenStart) {
  assert(tokenStart.type_ == TokenType::LeftParen && (char)fstream_.peek() == Tokens::LeftParen);
  char c;
  while (c = getNextChar(), c != Tokens::EndOfFile && c != Tokens::NewLine) {
    tokenStart.data_.push_back(c);
  }
  if (c != Tokens::NewLine) {
    throwInvalidArgumentException(tokenStart, "Malformed single line comment -- Missing new line character");
  }
  tokenStart.data_.push_back(c);
  tokenStart.type_ = TokenType::SingleLineComment;
}

void Tokenizer::parseString(Token &tokenStart) {
  assert(tokenStart.type_ == TokenType::DoubleQuote);
  char c;
  while (c = getNextChar(), c != Tokens::EndOfFile && c != Tokens::DoubleQuote) {
    tokenStart.data_.push_back(c);
    if (c == Tokens::RightParen && (char)fstream_.peek() == Tokens::DoubleQuote) {
      c = getNextChar();
      tokenStart.data_.push_back(c);
    }
  }
  if (c != Tokens::DoubleQuote) {
    throwInvalidArgumentException(tokenStart, "Malformed string -- missing closing double quote");
  }
  tokenStart.data_.push_back(c);
  tokenStart.type_ = TokenType::None;
}

void Tokenizer::parseBlockComment(Token &tokenStart) {
  assert(tokenStart.type_ == TokenType::LeftParen && (char)fstream_.peek() == Tokens::Asterisk);
  char c;
  while (c = getNextChar(), c != Tokens::EndOfFile) {
    tokenStart.data_.push_back(c);
    if (c == Tokens::Asterisk && (char)fstream_.peek() == Tokens::LeftParen) {
      c = getNextChar();
      break;
    }
  }
  if (c != Tokens::LeftParen) {
    throwInvalidArgumentException(tokenStart, "Malformed block comment -- Missing \"*/\" delimiter");
  }
  tokenStart.data_.push_back(c);
  tokenStart.type_ = TokenType::BlockComment;
}

std::invalid_argument& Tokenizer::throwInvalidArgumentException(Token& token, std::string&& message) {
  throw std::invalid_argument("(Line " +
                        std::to_string(token.getLineNumber()) +
                        ":" + std::to_string(token.getCharColumn()) +
                        ") "+ message);
}

