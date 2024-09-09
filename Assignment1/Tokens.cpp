#include "Tokens.hpp"

namespace Tokens {
const std::string getTokenValue(const TokenType &type) {
  switch (type) {
  case TokenType::Null:
    return "Null";
  case TokenType::EndOfFile:
    return "EOF";
  case TokenType::NewLine:
    return "NewLine";
  case TokenType::Tab:
    return "Tab";
  case TokenType::CR:
    return "CarriageReturn";
  case TokenType::Digit:
    return "Digit";
  case TokenType::EscapeCharacter:
    return "EscapeCharacter";
  case TokenType::Letter:
    return "Letter";
  case TokenType::HexDigit:
    return "HexDigit";
  case TokenType::LeftParen:
    return "LeftParen";
  case TokenType::RightParen:
    return "RightParen";
  case TokenType::LeftBracket:
    return "LeftBracket";
  case TokenType::RightBracket:
    return "RightBracket";
  case TokenType::LeftBrace:
    return "LeftBrace";
  case TokenType::RightBrace:
    return "RightBrace";
  case TokenType::DoubleQuote:
    return "DoubleQuote";
  case TokenType::SingleQuote:
    return "SingleQuote";
  case TokenType::Comma:
    return "Comma";
  case TokenType::Plus:
    return "Plus";
  case TokenType::Minus:
    return "Minus";
  case TokenType::Asterisk:
    return "Asterisk";
  case TokenType::Divide:
    return "Divide";
  case TokenType::Modulo:
    return "Modulo";
  case TokenType::Caret:
    return "Caret";
  case TokenType::LessThan:
    return "LessThan";
  case TokenType::GreaterThan:
    return "GreaterThan";
  case TokenType::LessThanEqual:
    return "LessThanEqual";
  case TokenType::GreaterThanEqual:
    return "GreaterThanEqual";
  case TokenType::BooleanAnd:
    return "BooleanAnd";
  case TokenType::BooleanOr:
    return "BooleanOr";
  case TokenType::BooleanNot:
    return "BooleanNot";
  case TokenType::BooleanEqual:
    return "BooleanEqual";
  case TokenType::BooleanNotEqual:
    return "BooleanNotEqual";
  case TokenType::BooleanTrue:
    return "BooleanTrue";
  case TokenType::BooleanFalse:
    return "BooleanFalse";
  case TokenType::SingleLineComment:
    return "SingleLineComment";
  case TokenType::BlockCommentStart:
    return "BlockCommentStart";
  case TokenType::BlockCommentEnd:
    return "BlockCommentEnd";
  default:
    return "Uknown Token";
  }
}
} // namespace Tokens
