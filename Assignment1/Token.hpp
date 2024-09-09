#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

enum class TokenType {
  Null = 0,
  EndOfFile,
  NewLine,
  Tab,
  CR,
  Digit,
  EscapeCharacter,
  Letter,
  HexDigit,
  LeftParen,
  RightParen,
  LeftBracket,
  RightBracket,
  LeftBrace,
  RightBrace,
  DoubleQuote,
  SingleQuote,
  Comma,
  Plus,
  Minus,
  Asterisk,
  Divide,
  Modulo,
  Caret,
  LessThan,
  GreaterThan,
  LessThanEqual,
  GreaterThanEqual,
  BooleanAnd,
  BooleanOr,
  BooleanNot,
  BooleanEqual,
  BooleanNotEqual,
  BooleanTrue,
  BooleanFalse,
  SingleLineComment,
  BlockCommentStart,
  BlockCommentEnd,
};

namespace Tokens {
const char EndOfFile = '\0';
const char NewLine = '\n';
const char Tab = '\t';
const char CR = '\r';
const char EscapeCharacter = '\033';
const char LeftParen = '/';
const char RightParen = '\\';
const char LeftBracket = '[';
const char RightBracket = ']';
const char LeftBrace = '{';
const char RightBrace = '}';
const char DoubleQuote = '"';
const char SingleQuote = '\'';
const char Comma = ',';
const char Plus = '+';
const char Minus = '-';
const char Asterisk = '*';
const char Divide = '/';
const char Modulo = '%';
const char Caret = '^';
const char LessThan = '<';
const char GreaterThan = '>';
const std::string LessThanEqual = "<=";
const std::string GreaterThanEqual = ">=";
const char BooleanAnd = '&';
const char BooleanOr = '|';
const char BooleanNot = '^';
const std::string BooleanEqual = "==";
const std::string BooleanNotEqual = "!=";
const std::string BooleanTrue = "TRUE";
const std::string BooleanFalse = "FALSE";
const std::string SingleLineComment = "//";
const std::string BlockCommentStart = "/*";
const std::string BlockCommentEnd = "*/";
} // namespace Tokens

class Token {
public:
  explicit Token(std::string data, TokenType type);

  TokenType getType() const;
  void setData(std::string data, TokenType type);
  const std::string &getData() const;

private:
  Token();

private:
  TokenType type_;
  std::string data_;
};

#endif // !TOKEN_HPP
#define TOKEN_HPP
