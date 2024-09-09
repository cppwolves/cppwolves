#include "Tokens.hpp"
#include "Token.hpp"
#include <cctype>
#include <unordered_map>

static const std::unordered_map<TokenType, std::string> sTokenNameMap{
    {TokenType::None, "None"},
    {TokenType::EndOfFile, "EndOfFile"},
    {TokenType::NewLine, "NewLine"},
    {TokenType::Tab, "Tab"},
    {TokenType::CR, "CarriageReturn"},
    {TokenType::Digit, "Digit"},
    {TokenType::EscapeCharacter, "EscapeCharacter"},
    {TokenType::Letter, "Letter"},
    {TokenType::HexDigit, "HexDigit"},
    {TokenType::LeftParen, "LeftParen"},
    {TokenType::RightParen, "RightParen"},
    {TokenType::LeftBracket, "LeftBracket"},
    {TokenType::RightBracket, "RightBracket"},
    {TokenType::LeftBrace, "LeftBrace"},
    {TokenType::RightBrace, "RightBrace"},
    {TokenType::DoubleQuote, "DoubleQuote"},
    {TokenType::SingleQuote, "SingleQuote"},
    {TokenType::Comma, "Comma"},
    {TokenType::Plus, "Plus"},
    {TokenType::Minus, "Minus"},
    {TokenType::Asterisk, "Asterisk"},
    {TokenType::Divide, "Divide"},
    {TokenType::Modulo, "Modulo"},
    {TokenType::Caret, "Caret"},
    {TokenType::LessThan, "LessThan"},
    {TokenType::GreaterThan, "GreaterThan"},
    {TokenType::LessThanEqual, "LessThanEqual"},
    {TokenType::GreaterThanEqual, "GreaterThanEqual"},
    {TokenType::BooleanAnd, "BooleanAnd"},
    {TokenType::BooleanOr, "BooleanOr"},
    {TokenType::BooleanNot, "BooleanNot"},
    {TokenType::BooleanEqual, "BooleanEqual"},
    {TokenType::BooleanNotEqual, "BooleanNotEqual"},
    {TokenType::BooleanTrue, "BooleanTrue"},
    {TokenType::BooleanFalse, "BooleanFalse"},
    {TokenType::SingleLineComment, "SingleLineComment"},
    {TokenType::BlockCommentStart, "BlockCommentStart"},
    {TokenType::BlockCommentEnd, "BlockCommentEnd"},
};

static const std::unordered_map<TokenType, std::string> sTokenValueMap{
    {TokenType::None, Tokens::None},
    {TokenType::EndOfFile, Tokens::EndOfFile},
    {TokenType::NewLine, Tokens::NewLine},
    {TokenType::Tab, Tokens::Tab},
    {TokenType::CR, Tokens::CR},
    {TokenType::EscapeCharacter, Tokens::EscapeCharacter},
    {TokenType::LeftParen, Tokens::LeftParen},
    {TokenType::RightParen, Tokens::RightParen},
    {TokenType::LeftBracket, Tokens::LeftBracket},
    {TokenType::RightBracket, Tokens::RightBracket},
    {TokenType::LeftBrace, Tokens::LeftBrace},
    {TokenType::RightBrace, Tokens::RightBrace},
    {TokenType::DoubleQuote, Tokens::DoubleQuote},
    {TokenType::SingleQuote, Tokens::SingleQuote},
    {TokenType::Comma, Tokens::Comma},
    {TokenType::Plus, Tokens::Plus},
    {TokenType::Minus, Tokens::Minus},
    {TokenType::Asterisk, Tokens::Asterisk},
    {TokenType::Divide, Tokens::Divide},
    {TokenType::Modulo, Tokens::Modulo},
    {TokenType::Caret, Tokens::Caret},
    {TokenType::LessThan, Tokens::LessThan},
    {TokenType::GreaterThan, Tokens::GreaterThan},
    {TokenType::LessThanEqual, Tokens::LessThanEqual},
    {TokenType::GreaterThanEqual, Tokens::GreaterThanEqual},
    {TokenType::BooleanAnd, Tokens::BooleanAnd},
    {TokenType::BooleanOr, Tokens::BooleanOr},
    {TokenType::BooleanNot, Tokens::BooleanNot},
    {TokenType::BooleanEqual, Tokens::BooleanEqual},
    {TokenType::BooleanNotEqual, Tokens::BooleanNotEqual},
    {TokenType::BooleanTrue, Tokens::BooleanTrue},
    {TokenType::BooleanFalse, Tokens::BooleanFalse},
    {TokenType::SingleLineComment, Tokens::SingleLineComment},
    {TokenType::BlockCommentStart, Tokens::BlockCommentStart},
    {TokenType::BlockCommentEnd, Tokens::BlockCommentEnd},
};
static const std::unordered_map<std::string, TokenType> sTokenTypeMap{
    {Tokens::None, TokenType::None},
    {Tokens::EndOfFile, TokenType::EndOfFile},
    {Tokens::NewLine, TokenType::NewLine},
    {Tokens::Tab, TokenType::Tab},
    {Tokens::CR, TokenType::CR},
    // {Tokens::Digit, "Digit"},
    {Tokens::EscapeCharacter, TokenType::EscapeCharacter},
    // {Tokens::Letter, "Letter"},
    // {Tokens::HexDigit, "HexDigit"},
    {Tokens::LeftParen, TokenType::LeftParen},
    {Tokens::RightParen, TokenType::RightParen},
    {Tokens::LeftBracket, TokenType::LeftBracket},
    {Tokens::RightBracket, TokenType::RightBracket},
    {Tokens::LeftBrace, TokenType::LeftBrace},
    {Tokens::RightBrace, TokenType::RightBrace},
    {Tokens::DoubleQuote, TokenType::DoubleQuote},
    {Tokens::SingleQuote, TokenType::SingleQuote},
    {Tokens::Comma, TokenType::Comma},
    {Tokens::Plus, TokenType::Plus},
    {Tokens::Minus, TokenType::Minus},
    {Tokens::Asterisk, TokenType::Asterisk},
    {Tokens::Divide, TokenType::Divide},
    {Tokens::Modulo, TokenType::Modulo},
    {Tokens::Caret, TokenType::Caret},
    {Tokens::LessThan, TokenType::LessThan},
    {Tokens::GreaterThan, TokenType::GreaterThan},
    {Tokens::LessThanEqual, TokenType::LessThanEqual},
    {Tokens::GreaterThanEqual, TokenType::GreaterThanEqual},
    {Tokens::BooleanAnd, TokenType::BooleanAnd},
    {Tokens::BooleanOr, TokenType::BooleanOr},
    {Tokens::BooleanNot, TokenType::BooleanNot},
    {Tokens::BooleanEqual, TokenType::BooleanEqual},
    {Tokens::BooleanNotEqual, TokenType::BooleanNotEqual},
    {Tokens::BooleanTrue, TokenType::BooleanTrue},
    {Tokens::BooleanFalse, TokenType::BooleanFalse},
    {Tokens::SingleLineComment, TokenType::SingleLineComment},
    {Tokens::BlockCommentStart, TokenType::BlockCommentStart},
    {Tokens::BlockCommentEnd, TokenType::BlockCommentEnd},
};

namespace Tokens {
const std::string getTokenName(const TokenType &type) {
  if (auto val = sTokenNameMap.find(type); val != sTokenNameMap.end()) {
    return val->second;
  }
  return sTokenNameMap.at(TokenType::None);
}

const std::string getTokenName(const std::string &token) {
  return getTokenName(getTokenType(token));
}

TokenType getTokenType(const std::string &value) {
  if (auto type = sTokenTypeMap.find(value); type != sTokenTypeMap.end()) {
    return type->second;
  } else if (value.size() == 1 && std::isalnum((char)value[0])) {
    if (std::isalpha((char)value[0])) {
      return TokenType::Letter;
    } else {
      return TokenType::Digit;
    }
  }
  return sTokenTypeMap.at("None");
}

std::string getTokenValue(const TokenType &token) {
  if (auto value = sTokenValueMap.find(token); value != sTokenValueMap.end()) {
    return value->second;
  }
  return sTokenValueMap.at(TokenType::None);
}
} // namespace Tokens
