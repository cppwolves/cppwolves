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
    last_row_ = row_ - 1;
    index_ = token.char_index_;
    last_index_ = index_ - 1;
    col_ = token.char_col_;
    last_col_ = col_ - 1;

    return token;
  }

  Token token = getNextCharToken();
  switch (token.type_) {
  case TokenType::LeftSlash: {
    char c = getNextChar();
    switch (c) {
    case Tokens::LeftSlash:
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
  case TokenType::Asterisk:
    if (fstream_.peek() == Tokens::LeftSlash) {
      printErrorAndExit(
          "ERROR: Program contains C-style, unterminated comment on line " +
          std::to_string(token.getLineNumber()));
    }
    break;
  default:
    break;
  }

  return token;
}

void Tokenizer::putBackToken(Token token) { token_stack_.push_back(token); }

void Tokenizer::putBackChar(char c) {
  assert(index_ >= 0 &&
         "Cannot put back char -- already at beginning of stream");
  assert(fstream_.putback(c) && "Putback failed");
  fstream_.flush();

  if (c == Tokens::NewLine) {
    row_ = last_row_;
    last_row_ = row_ - 1;
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
  char c;
  TokenType token_type = TokenType::None;

  if (fstream_.eof()) {
    token_type = TokenType::EndOfFile;
  } else {
    do {
      c = getNextChar();
      TokenType type = Tokens::getTokenType(c);
      if (type != TokenType::None) {
        token_type = type;
        break;
      }
    } while (fstream_.good() && c != Tokens::EndOfFile);
  }

  Token token = Token(c, row_, col_, index_, token_type);
  return token;
}

void Tokenizer::parseSingleLineComment(Token &tokenStart) {
  assert(tokenStart.type_ == TokenType::LeftSlash &&
         (char)fstream_.peek() == Tokens::LeftSlash);
  char c = getNextChar();
  while (c != Tokens::EndOfFile && c != Tokens::NewLine) {
    tokenStart.data_.push_back(c);
    std::cout << " "; // TEMP FOR IGNORE COMMENTS
    c = getNextChar();
  }
  if (c != Tokens::NewLine) {
    printErrorAndExit(
        "ERROR: Program contains C-style, unterminated comment on line " +
        std::to_string(tokenStart.getLineNumber()));
  }
  tokenStart.data_.push_back(c);
  tokenStart.type_ = TokenType::SingleLineComment;
  std::cout << " " << std::endl; // TEMP FOR IGNORE COMMENTS
}

void Tokenizer::parseString(Token &tokenStart) {
  assert(tokenStart.type_ == TokenType::DoubleQuote);
  char c = getNextChar();
  while (c != Tokens::EndOfFile && c != Tokens::DoubleQuote) {
    tokenStart.data_.push_back(c);
    if (c == Tokens::RightSlash &&
        (char)fstream_.peek() == Tokens::DoubleQuote) {
      c = getNextChar();
      tokenStart.data_.push_back(c);
    }
    c = getNextChar();
  }
  if (c != Tokens::DoubleQuote) {
    printErrorAndExit("ERROR: Program contains unterminated string on line " +
                      std::to_string(tokenStart.getLineNumber()));
  }
  tokenStart.data_.push_back(c);
  tokenStart.type_ = TokenType::None;
}

void Tokenizer::parseBlockComment(Token &tokenStart) {
  assert(tokenStart.type_ == TokenType::LeftSlash &&
         (char)fstream_.peek() == Tokens::Asterisk);
  char c = getNextChar();
  // std::cout << " " << std::flush; // TEMP FOR IGNORE COMMENTS
  while (c != Tokens::EndOfFile) {
    c == Tokens::NewLine
        ? std::cout << std::endl
        : std::cout << " " << std::flush; // TEMP FOR IGNORE COMMENTS
    tokenStart.data_.push_back(c);
    if (c == Tokens::Asterisk && (char)fstream_.peek() == Tokens::LeftSlash) {
      c = getNextChar();
      break;
    }
    c = getNextChar();
  }
  if (c != Tokens::LeftSlash) {
    printErrorAndExit(
        "ERROR: Program contains C-style, unterminated comment on line " +
        std::to_string(tokenStart.getLineNumber()));
  }
  tokenStart.data_.push_back(c);
  tokenStart.type_ = TokenType::BlockComment;
}

void Tokenizer::printErrorAndExit(std::string &&message) {
  std::cerr << message << '\n';
  exit(1);
}
