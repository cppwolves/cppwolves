#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <string>

enum class TokenType {
  None = 0,
  EndOfFile,
  NewLine,
  Tab,
  CR,
  Digit,
  EscapeCharacter,
  Letter,
  HexDigit,
  LeftSlash,
  RightSlash,
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
  BlockComment,
  BlockCommentStart,
  BlockCommentEnd,
};

namespace Tokens {
static const char EndOfFile = '\0';
static const char NewLine = '\n';
static const char Tab = '\t';
static const char CR = '\r';
static const char EscapeCharacter = '\033';
static const char LeftSlash = '/';
static const char RightSlash = '\\';
static const char LeftParen = '(';
static const char RightParen = ')';
static const char LeftBracket = '[';
static const char RightBracket = ']';
static const char LeftBrace = '{';
static const char RightBrace = '}';
static const char DoubleQuote = '"';
static const char SingleQuote = '\'';
static const char Comma = ',';
static const char Plus = '+';
static const char Minus = '-';
static const char Asterisk = '*';
static const char Divide = '/';
static const char Modulo = '%';
static const char Caret = '^';
static const char LessThan = '<';
static const char GreaterThan = '>';
static const char BooleanNot = '!';

static const std::string None = "None";
static const std::string LessThanEqual = "<=";
static const std::string GreaterThanEqual = ">=";
static const std::string BooleanEqual = "==";
static const std::string BooleanNotEqual = "!=";
static const std::string BooleanAnd = "&&";
static const std::string BooleanOr = "||";
static const std::string BooleanTrue = "TRUE";
static const std::string BooleanFalse = "FALSE";
static const std::string SingleLineCommentStart = "//";
static const std::string BlockCommentStart = "/*";
static const std::string BlockCommentEnd = "*/";
} // namespace Tokens

namespace Tokens {
const std::string getTokenName(const TokenType &token);
const std::string getTokenName(const std::string &token);
const std::string getTokenName(char token);

TokenType getTokenType(const std::string &value);
TokenType getTokenType(char value);

std::string getTokenValue(const TokenType &token);
} // namespace Tokens

#endif // TOKENS_HPP
