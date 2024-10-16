// tokenizer.cpp
#include "tokenizer.hpp"
#include "token_enum.hpp"
#include "token_error.hpp"
#include "token_node.hpp"
#include <cctype>
#include <unordered_set>

Tokenizer::Tokenizer(const std::string &filename)
    : lineNumber(1), endOfFile(false) {
  inputFile.open(filename);
  if (!inputFile.is_open()) {
    throw std::runtime_error("Could not open file " + filename);
  }
  advance(); // Initialize currentChar
}

void Tokenizer::advance() {
  if (inputFile.get(currentChar)) {
    if (currentChar == '\n') {
      lineNumber++;
    }
  } else {
    endOfFile = true;
  }
}

void Tokenizer::skipWhitespace() {
  while (!endOfFile && std::isspace(currentChar)) {
    advance();
  }
}

void Tokenizer::skipComment() {
  // Single-line or multi-line comments
  if (currentChar == '/') {
    advance();
    if (currentChar == '/') {
      // Single-line comment
      while (!endOfFile && currentChar != '\n') {
        advance();
      }
    } else if (currentChar == '*') {
      // Multi-line comment
      advance();
      while (!endOfFile) {
        if (currentChar == '*') {
          advance();
          if (currentChar == '/') {
            advance();
            break;
          }
        } else {
          advance();
        }
      }
    } else {
      // Not a comment, return to previous position
      inputFile.unget();
      currentChar = '/';
    }
  }
}

bool Tokenizer::isKeyword(const std::string &str) {
  static const std::unordered_set<std::string> keywords = {
      "function", "procedure", "if",     "else",   "for",  "while",
      "return",   "char",      "int",    "bool",   "TRUE", "FALSE",
      "void",     "main",      "printf", "getchar"};
  return keywords.find(str) != keywords.end();
}

bool Tokenizer::isLetter(char ch) {
  return std::isalpha(static_cast<unsigned char>(ch)) || ch == '_';
}

bool Tokenizer::isDigit(char ch) {
  return std::isdigit(static_cast<unsigned char>(ch));
}

bool Tokenizer::isHexDigit(char ch) {
  return std::isxdigit(static_cast<unsigned char>(ch));
}

bool Tokenizer::isOperatorChar(char ch) {
  return std::string("=+-*/%^<>!&|").find(ch) != std::string::npos;
}

Token Tokenizer::identifierOrKeyword() {
  std::string lexeme;
  int startLine = lineNumber;

  while (!endOfFile && (isLetter(currentChar) || isDigit(currentChar))) {
    lexeme += currentChar;
    advance();
  }

  if (isKeyword(lexeme)) {
    TokenType type;
    if (lexeme == "function")
      type = TokenType::FUNCTION;
    else if (lexeme == "procedure")
      type = TokenType::PROCEDURE;
    else if (lexeme == "if")
      type = TokenType::IF;
    else if (lexeme == "else")
      type = TokenType::ELSE;
    else if (lexeme == "for")
      type = TokenType::FOR;
    else if (lexeme == "while")
      type = TokenType::WHILE;
    else if (lexeme == "return")
      type = TokenType::RETURN;
    else if (lexeme == "char")
      type = TokenType::CHAR;
    else if (lexeme == "int")
      type = TokenType::INT;
    else if (lexeme == "bool")
      type = TokenType::BOOL;
    else if (lexeme == "TRUE")
      type = TokenType::TRUE;
    else if (lexeme == "FALSE")
      type = TokenType::FALSE;
    else if (lexeme == "void")
      type = TokenType::VOID;
    else if (lexeme == "main")
      type = TokenType::MAIN;
    else if (lexeme == "printf")
      type = TokenType::PRINTF;
    else if (lexeme == "getchar")
      type = TokenType::GETCHAR;
    else
      type = TokenType::IDENTIFIER; // Fallback
    return Token(type, lexeme, startLine);
  } else {
    return Token(TokenType::IDENTIFIER, lexeme, startLine);
  }
}

Token Tokenizer::number() {
  std::string lexeme;
  int startLine = lineNumber;

  // Handle optional leading sign
  if (currentChar == '+' || currentChar == '-') {
    lexeme += currentChar;
    advance();
    if (!isDigit(currentChar)) {
      // Sign not followed by a digit; invalid number
      return Token(TokenType::INVALID_TOKEN, lexeme, startLine);
    }
  }

  if (!isDigit(currentChar)) {
    // Invalid integer
    lexeme += currentChar;
    advance();
    return Token(TokenType::INVALID_TOKEN, lexeme, startLine);
  }

  while (!endOfFile && isDigit(currentChar)) {
    lexeme += currentChar;
    advance();
  }

  // Check for invalid characters after digits
  if (isLetter(currentChar)) {
    // Invalid integer: contains letters after digits
    while (!endOfFile && (isLetter(currentChar) || isDigit(currentChar))) {
      lexeme += currentChar;
      advance();
    }
    // Return an invalid token with the full lexeme
    return Token(TokenType::INVALID_TOKEN, lexeme, startLine);
  }

  return Token(TokenType::INTEGER, lexeme, startLine);
}

Token Tokenizer::stringLiteral() {
  std::string lexeme;
  int startLine = lineNumber;
  char quoteType = currentChar; // ' or "
  lexeme += currentChar;        // Include the starting quote
  advance();

  while (!endOfFile && currentChar != quoteType) {
    if (currentChar == '\\') {
      // Handle escape sequence
      lexeme += currentChar;
      advance();
      if (!endOfFile) {
        lexeme += currentChar;
        advance();
      }
    } else {
      lexeme += currentChar;
      advance();
    }
  }

  if (currentChar == quoteType) {
    lexeme += currentChar; // Include the closing quote
    advance();
    TokenType type =
        (quoteType == '"') ? TokenType::STRING : TokenType::CHAR_LITERAL;
    return Token(type, lexeme, startLine);
  } else {
    // Unterminated string
    return Token(TokenType::INVALID_TOKEN, lexeme, startLine);
  }
}

Token Tokenizer::charLiteral() { return stringLiteral(); }

Token Tokenizer::operatorOrDelimiter() {
  int startLine = lineNumber;
  std::string lexeme;
  lexeme += currentChar;
  char firstChar = currentChar;

  // Handle two-character operators
  if (currentChar == '=' || currentChar == '!' || currentChar == '<' ||
      currentChar == '>') {
    advance();
    if (currentChar == '=') {
      lexeme += currentChar;
      advance();
      if (lexeme == "==")
        return Token(TokenType::BOOLEAN_EQUAL, lexeme, startLine);
      if (lexeme == "!=")
        return Token(TokenType::BOOLEAN_NOT_EQUAL, lexeme, startLine);
      if (lexeme == "<=")
        return Token(TokenType::LT_EQUAL, lexeme, startLine);
      if (lexeme == ">=")
        return Token(TokenType::GT_EQUAL, lexeme, startLine);
    } else {
      // Single-character operators
      if (lexeme == "=")
        return Token(TokenType::ASSIGNMENT_OPERATOR, lexeme, startLine);
      if (lexeme == "!")
        return Token(TokenType::BOOLEAN_NOT, lexeme, startLine);
      if (lexeme == "<")
        return Token(TokenType::LT, lexeme, startLine);
      if (lexeme == ">")
        return Token(TokenType::GT, lexeme, startLine);
      // The current character is not part of the operator, so do not advance
      // inputFile.unget();
      return Token(TokenType::INVALID_TOKEN, lexeme, startLine);
    }
  } else if (currentChar == '&' || currentChar == '|') {
    advance();
    if (currentChar == firstChar) {
      lexeme += currentChar;
      advance();
      if (lexeme == "&&")
        return Token(TokenType::BOOLEAN_AND, lexeme, startLine);
      if (lexeme == "||")
        return Token(TokenType::BOOLEAN_OR, lexeme, startLine);
    } else {
      // Invalid operator
      return Token(TokenType::INVALID_TOKEN, lexeme, startLine);
    }
  } else {
    // Other single-character operators or delimiters
    TokenType type;
    switch (firstChar) {
    case '+':
      type = TokenType::PLUS;
      break;
    case '-':
      type = TokenType::MINUS;
      break;
    case '*':
      type = TokenType::ASTERISK;
      break;
    case '/':
      type = TokenType::DIVIDE;
      break;
    case '%':
      type = TokenType::MODULO;
      break;
    case '^':
      type = TokenType::CARET;
      break;
    case '(':
      type = TokenType::L_PAREN;
      break;
    case ')':
      type = TokenType::R_PAREN;
      break;
    case '[':
      type = TokenType::L_BRACKET;
      break;
    case ']':
      type = TokenType::R_BRACKET;
      break;
    case '{':
      type = TokenType::L_BRACE;
      break;
    case '}':
      type = TokenType::R_BRACE;
      break;
    case ';':
      type = TokenType::SEMICOLON;
      break;
    case ',':
      type = TokenType::COMMA;
      break;
    case '\'':
      type = TokenType::SINGLE_QUOTE;
      break;
    case '"':
      type = TokenType::DOUBLE_QUOTE;
      break;
    default:
      type = TokenType::INVALID_TOKEN;
      break;
    }
    advance();
    if (type != TokenType::INVALID_TOKEN) {
      return Token(type, lexeme, startLine);
    } else {
      return Token(TokenType::INVALID_TOKEN, lexeme, startLine);
    }
  }

  return Token(TokenType::INVALID_TOKEN, lexeme, startLine);
}

Token Tokenizer::getNextToken() {
  // If there are tokens in the queue, return the next one
  if (!tokenQueue.empty()) {
    Token token = tokenQueue.front();
    tokenQueue.pop();
    return token;
  }

  skipWhitespace();

  while (!endOfFile && currentChar == '/') {
    // Possible comment
    advance();
    if (currentChar == '/' || currentChar == '*') {
      inputFile.unget();
      currentChar = '/';
      skipComment();
      skipWhitespace();
    } else {
      inputFile.unget();
      currentChar = '/';
      break;
    }
  }

  if (endOfFile) {
    return Token(TokenType::END_OF_FILE, "", lineNumber);
  }

  if (isLetter(currentChar) || currentChar == '_') {
    return identifierOrKeyword();
  }

  if (isDigit(currentChar)) {
    return number();
  }

  if (currentChar == '+' || currentChar == '-') {
    // Determine if this is a number or an operator
    char nextChar = inputFile.peek();
    if (isDigit(nextChar)) {
      return number();
    } else {
      return operatorOrDelimiter();
    }
  }

  if (currentChar == '"' || currentChar == '\'') {
    // Handle string or char literal
    char quoteType = currentChar;
    int startLine = lineNumber;

    // Create token for opening quote
    TokenType quoteTokenType =
        (quoteType == '"') ? TokenType::DOUBLE_QUOTE : TokenType::SINGLE_QUOTE;
    Token openingQuoteToken(quoteTokenType, std::string(1, quoteType),
                            startLine);
    advance(); // Move past opening quote

    // Now read the string content
    std::string lexeme;
    while (!endOfFile && currentChar != quoteType) {
      if (currentChar == '\\') {
        // Handle escape sequence
        lexeme += currentChar;
        advance();
        if (!endOfFile) {
          lexeme += currentChar;
          advance();
        }
      } else {
        lexeme += currentChar;
        advance();
      }
    }

    if (currentChar == quoteType) {
      // Create token for string content
      TokenType contentTokenType =
          (quoteType == '"') ? TokenType::STRING : TokenType::CHAR_LITERAL;
      Token contentToken(contentTokenType, lexeme, startLine);

      // Create token for closing quote
      Token closingQuoteToken(quoteTokenType, std::string(1, quoteType),
                              lineNumber);
      advance(); // Move past closing quote

      // Enqueue the tokens to the tokenQueue
      tokenQueue.push(contentToken);
      tokenQueue.push(closingQuoteToken);

      // Return the opening quote token
      return openingQuoteToken;

    } else {
      // Unterminated string
      throwUnterminatedStringError(
          Token(TokenType::INVALID_TOKEN, lexeme, startLine));
      // return Token(TokenType::INVALID_TOKEN, lexeme, startLine);
    }
  }

  if (isOperatorChar(currentChar) ||
      std::string("()[]{};,").find(currentChar) != std::string::npos) {
    return operatorOrDelimiter();
  }

  // Invalid character
  std::string lexeme(1, currentChar);
  int startLine = lineNumber;
  advance();
  return Token(TokenType::INVALID_TOKEN, lexeme, startLine);
}

std::vector<Token> Tokenizer::tokenize() {
  std::vector<Token> tokens;
  Token token = getNextToken();

  while (token.type != TokenType::END_OF_FILE) {
    if (token.type == TokenType::INVALID_TOKEN) {
      // Output error message and stop tokenization
      std::string errorMessage = "Syntax error on line " +
                                 std::to_string(token.lineNumber) +
                                 ": invalid integer '" + token.lexeme + "'\n";
      // Store the error message for later use
      this->errorMessage = errorMessage;
      // Clear the tokens vector to ensure no tokens are outputted
      tokens.clear();
      return tokens;
    }
    tokens.push_back(token);
    token = getNextToken();
  }

  return tokens;
}
