#include "Tokenizer.hpp"
#include "Tokens.hpp"
#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>

Tokenizer::Tokenizer()
    : row_(0), index_(-1), col_(-1), ifstream_(nullptr),
      state_(NOT_INITIALIZED) {}

Tokenizer::Tokenizer(std::string filename)
    : row_(0), index_(-1), col_(-1), filename_(std::move(filename)),
      ifstream_({}), state_(READ_START) {
  ifstream_.open(filename_);
  assert(ifstream_.is_open() && "Could not open file");
  ifstream_.seekg(0);
}

Tokenizer::~Tokenizer() { ifstream_.close(); }

void Tokenizer::setTokenData(Token *token, const std::string &data,
                             uint32_t lineNumber, uint32_t charColumn,
                             uint32_t charIndex, TokenType type) {
  token->setData(data, lineNumber, charColumn, charIndex, type);
}

void Tokenizer::stateReadComment(Token *token, char &c) {
  if (c == Tokens::EndOfFile) {
    printErrorAndExit(EXIT_UNTERMINATED_COMMENT, token);
  }
  token->data_.push_back(c);

  switch (token->type_) {
  case TokenType::Divide:
  case TokenType::LeftSlash: {
    if (c == Tokens::LeftSlash) {
      token->type_ = TokenType::SingleLineComment;
      state_ = READ_SINGLE_LINE_COMMENT;
    } else if (c == Tokens::Asterisk) {
      token->type_ = TokenType::BlockComment;
      state_ = READ_BLOCK_COMMENT;
    }
    last_state_ = READ_COMMENT;
  } break;
  case TokenType::SingleLineComment: {
    if (c == Tokens::NewLine) {
      state_ = READ_END;
    } else {
      state_ = READ_SINGLE_LINE_COMMENT;
    }
    last_state_ = READ_COMMENT;
  } break;
  case TokenType::BlockComment: {
    if (c == Tokens::Asterisk && peekNextChar() == Tokens::LeftSlash) {
      token->data_.push_back(getNextChar());
      state_ = READ_END;
      last_state_ = READ_END;
    } else {
      state_ = READ_BLOCK_COMMENT;
      last_state_ = READ_COMMENT;
    }
  } break;
  default:
    break;
  }
}

void Tokenizer::stateReadChar(Token *token, char &c) {
  if ((std::isalnum(c) || c == '_') && (token->type_ == TokenType::Letter ||
                                        token->type_ == TokenType::String)) {
    token->data_.push_back(c);
    token->type_ = TokenType::String;
    state_ = READ_CHAR;
  } else {
    putBackChar(c);
    state_ = READ_END;
  }
}

void Tokenizer::stateReadDigit(Token *token, const char &c) {
  if (!std::isalnum(c)) {
    // This isn't a valid char
    state_ = READ_END;
    last_state_ = READ_END;
    putBackChar(c);
    return;
  }
  switch (token->type_) {
  case TokenType::Letter: {
    // We have a token that is a already a letter, so this might be a variable
    // name or similar
    token->data_.push_back(c);
    token->type_ = TokenType::String;
    state_ = READ_CHAR;
  } break;
  case TokenType::Number:
  case TokenType::Digit: {
    token->data_.push_back(c);
    token->type_ = TokenType::Number;
    state_ = READ_NUMBER;
  } break;
  case TokenType::Plus:
  case TokenType::Minus: {
    // This may be a number if it begins with a '-' or '+', otherwise get out
    if (Tokens::getTokenType(c) != TokenType::Digit) {
      putBackChar(c);
      state_ = READ_END;
    } else {
      token->data_.push_back(c);
      token->type_ = TokenType::Number;
      state_ = READ_NUMBER;
    }
  } break;
  default:
    // No idea what this was
    putBackChar(c);
    state_ = READ_END;
    last_state_ = READ_END;
    break;
  }
}

Token *Tokenizer::stateReadStart() {
  assert(state_ != NOT_INITIALIZED);
  char c;
  // Move past all whitespaces
  do {
    c = getNextChar();
  } while (c != Tokens::EndOfFile && c == Tokens::Space);

  TokenType type = Tokens::getTokenType(c);
  // Assume this will be the only char we'll collect
  state_ = READ_END;

  switch (type) {
  case TokenType::DoubleQuote:
    // If we are currently inside a string and see the ending '"', we're done
    last_state_ = last_state_ == READ_STRING ? READ_END : READ_STRING;
    break;
  case TokenType::LeftSlash: {
    // This may be a comment or divide
    char peek = peekNextChar();
    if (peek == Tokens::LeftSlash || peek == Tokens::Asterisk) {
      // We have a comment to read
      state_ = READ_COMMENT;
      last_state_ = READ_COMMENT;
    } else {
      type = TokenType::Divide;
    }
  } break;
  case TokenType::Asterisk: {
    if (peekNextChar() == Tokens::LeftSlash) {
      // Since we're not currently in a comment or string, this should not be
      // here.
      printErrorAndExit(EXIT_MISSING_BLOCK_COMMENT_START, nullptr);
    }
  } break;
  case TokenType::Letter:
    // Continue on with collecting chars
    state_ = READ_CHAR;
    break;
  case TokenType::Digit:
    // We are now collecting digits (numbers)
    state_ = READ_DIGIT;
    break;
  case TokenType::Plus:
  case TokenType::Minus:
    // We may have a -/+ number if the next char is a digit
    if (std::isdigit(peekNextChar())) {
      state_ = READ_NUMBER;
    }
    break;
  default:
    break;
  }
  return new Token(c, row_, col_, index_, type);
}

Token *Tokenizer::getNextToken() {
  Token *token = nullptr;
  state_ = READ_START;

  while (state_ != READ_END) {
    if (state_ == READ_START) {
      // Get our first token value and update the state
      token = stateReadStart();
    } else {
      char c = getNextChar();
      switch (state_) {
      case READ_CHAR:
        stateReadChar(token, c);
        break;
      case READ_NUMBER:
      case READ_DIGIT:
        stateReadDigit(token, c);
        break;
      case READ_COMMENT:
      case READ_SINGLE_LINE_COMMENT:
      case READ_BLOCK_COMMENT:
        stateReadComment(token, c);
        break;
      default:
        // Not sure what this was, so let's put it back and call it good
        putBackChar(c);
        state_ = READ_END;
        break;
      }
    }
  }
  return token;
}

char Tokenizer::peekNextChar() { return static_cast<char>(ifstream_.peek()); }

void Tokenizer::putBackChar(const char c) {
  assert(index_ >= 0 &&
         "Cannot put back char -- already at beginning of stream");
  assert(ifstream_.putback(c) && "Putback failed");

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
  if (ifstream_.eof()) {
    c = Tokens::EndOfFile;
  } else {
    ifstream_.get(c);
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

void Tokenizer::printErrorAndExit(const ExitCode code,
                                  const Token *token) const {
  std::string message;

  switch (code) {
  case EXIT_MISSING_BLOCK_COMMENT_START:
    message = "ERROR: Program missing opening C-style comment on line ";
    break;
  case EXIT_UNTERMINATED_COMMENT:
    message = "ERROR: Program contains C-style, unterminated comment on line ";
    break;
  case EXIT_UNTERMINATED_STRING:
    message = "ERROR: Program contains unterminated string on line ";
    break;
  default:
    message = "Error: ";
  }

  std::cerr << message << std::to_string(token ? token->line_number_ : row_)
            << '\n';
  exit(code);
}
