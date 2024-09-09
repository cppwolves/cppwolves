#ifndef TOKENS_HPP
#define TOKENS_HPP

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
const std::string Null = "Null";
const std::string EndOfFile = "\0";
const std::string NewLine = "\n";
const std::string Tab = "\t";
const std::string CR = "\r";
const std::string EscapeCharacter = "\033";
const std::string LeftParen = "/";
const std::string RightParen = "\\";
const std::string LeftBracket = "[";
const std::string RightBracket = "]";
const std::string LeftBrace = "{";
const std::string RightBrace = "}";
const std::string DoubleQuote = "\"";
const std::string SingleQuote = "\'";
const std::string Comma = ",";
const std::string Plus = "+";
const std::string Minus = "-";
const std::string Asterisk = "*";
const std::string Divide = "/";
const std::string Modulo = "%";
const std::string Caret = "^";
const std::string LessThan = "<";
const std::string GreaterThan = ">";
const std::string LessThanEqual = "<=";
const std::string GreaterThanEqual = ">=";
const std::string BooleanAnd = "&";
const std::string BooleanOr = "|";
const std::string BooleanNot = "^";
const std::string BooleanEqual = "==";
const std::string BooleanNotEqual = "!=";
const std::string BooleanTrue = "TRUE";
const std::string BooleanFalse = "FALSE";
const std::string SingleLineComment = "//";
const std::string BlockCommentStart = "/*";
const std::string BlockCommentEnd = "*/";
} // namespace Tokens

namespace Tokens {
const std::string getTokenValue(const TokenType &token);
} // namespace Tokens

#endif // TOKENS_HPP
