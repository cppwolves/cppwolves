#include "Tokens.hpp"
#include "Token.hpp"
#include <cctype>
#include <string>
#include <unordered_map>

static const std::unordered_map<TokenType, std::string> sTokenTypeNameMap{
    {TokenType::None, "None"},
    {TokenType::EndOfFile, "EndOfFile"},
    {TokenType::NewLine, "NewLine"},
    {TokenType::Tab, "Tab"},
    {TokenType::CR, "CarriageReturn"},
    {TokenType::Digit, "Digit"},
    {TokenType::EscapeCharacter, "EscapeCharacter"},
    {TokenType::Letter, "Letter"},
    {TokenType::HexDigit, "HexDigit"},
    {TokenType::LeftSlash, "LeftSlash"},
    {TokenType::RightSlash, "RightSlash"},
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
    {TokenType::BlockComment, "BlockComment"},
    {TokenType::BlockCommentStart, "BlockCommentStart"},
    {TokenType::BlockCommentEnd, "BlockCommentEnd"},
};

static const std::unordered_map<TokenType, char> sTokenTypeValueCharMap{
    {TokenType::EndOfFile, (char)Tokens::EndOfFile},
    {TokenType::NewLine, (char)Tokens::NewLine},
    {TokenType::Tab, (char)Tokens::Tab},
    {TokenType::CR, (char)Tokens::CR},
    {TokenType::EscapeCharacter, (char)Tokens::EscapeCharacter},
    {TokenType::LeftSlash, (char)Tokens::LeftSlash},
    {TokenType::RightSlash, (char)Tokens::RightSlash},
    {TokenType::LeftParen, (char)Tokens::LeftParen},
    {TokenType::RightParen, (char)Tokens::RightParen},
    {TokenType::LeftBracket, (char)Tokens::LeftBracket},
    {TokenType::RightBracket, (char)Tokens::RightBracket},
    {TokenType::LeftBrace, (char)Tokens::LeftBrace},
    {TokenType::RightBrace, (char)Tokens::RightBrace},
    {TokenType::DoubleQuote, (char)Tokens::DoubleQuote},
    {TokenType::SingleQuote, (char)Tokens::SingleQuote},
    {TokenType::Comma, (char)Tokens::Comma},
    {TokenType::Plus, (char)Tokens::Plus},
    {TokenType::Minus, (char)Tokens::Minus},
    {TokenType::Asterisk, (char)Tokens::Asterisk},
    {TokenType::Divide, (char)Tokens::Divide},
    {TokenType::Modulo, (char)Tokens::Modulo},
    {TokenType::Caret, (char)Tokens::Caret},
    {TokenType::LessThan, (char)Tokens::LessThan},
    {TokenType::GreaterThan, (char)Tokens::GreaterThan},
    {TokenType::BooleanNot, Tokens::BooleanNot},
};

static const std::unordered_map<TokenType, std::string> sTokenTypeValueMap{
    {TokenType::None, Tokens::None},
    {TokenType::GreaterThanEqual, Tokens::GreaterThanEqual},
    {TokenType::BooleanAnd, Tokens::BooleanAnd},
    {TokenType::BooleanOr, Tokens::BooleanOr},
    {TokenType::BooleanEqual, Tokens::BooleanEqual},
    {TokenType::BooleanNotEqual, Tokens::BooleanNotEqual},
    {TokenType::BooleanTrue, Tokens::BooleanTrue},
    {TokenType::BooleanFalse, Tokens::BooleanFalse},
    {TokenType::BlockCommentStart, Tokens::BlockCommentStart},
    {TokenType::BlockCommentEnd, Tokens::BlockCommentEnd},
};

static const std::unordered_map<char, TokenType> sTokenTypeCharMap{
    {Tokens::EndOfFile, TokenType::EndOfFile},
    {Tokens::NewLine, TokenType::NewLine},
    {Tokens::Tab, TokenType::Tab},
    {Tokens::CR, TokenType::CR},
    {Tokens::EscapeCharacter, TokenType::EscapeCharacter},
    {Tokens::LeftSlash, TokenType::LeftSlash},
    {Tokens::RightSlash, TokenType::RightSlash},
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
    {Tokens::BooleanNot, TokenType::BooleanNot},
};

static const std::unordered_map<std::string, TokenType> sTokenTypeMap{
    {Tokens::None, TokenType::None},
    {Tokens::LessThanEqual, TokenType::LessThanEqual},
    {Tokens::GreaterThanEqual, TokenType::GreaterThanEqual},
    {Tokens::BooleanAnd, TokenType::BooleanAnd},
    {Tokens::BooleanOr, TokenType::BooleanOr},
    {Tokens::BooleanEqual, TokenType::BooleanEqual},
    {Tokens::BooleanNotEqual, TokenType::BooleanNotEqual},
    {Tokens::BooleanTrue, TokenType::BooleanTrue},
    {Tokens::BooleanFalse, TokenType::BooleanFalse},
    {Tokens::BlockCommentStart, TokenType::BlockCommentStart},
    {Tokens::BlockCommentEnd, TokenType::BlockCommentEnd},
};

namespace Tokens {
const std::string getTokenName(const TokenType &type) {
  if (auto val = sTokenTypeNameMap.find(type); val != sTokenTypeNameMap.end()) {
    return val->second;
  }
  return sTokenTypeNameMap.at(TokenType::None);
}

const std::string getTokenName(const std::string &token) {
  return getTokenName(getTokenType(token));
}

const std::string getTokenName(char token) {
  return getTokenName(getTokenType(token));
}

TokenType getTokenType(const std::string &value) {
  auto type = sTokenTypeMap.find(value);
  if (type != sTokenTypeMap.end()) {
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

TokenType getTokenType(char value) {
  auto type = sTokenTypeCharMap.find(value);
  if (type != sTokenTypeCharMap.end()) {
    return type->second;
  } else if (std::isalnum(value)) {
    if (std::isalpha(value)) {
      return TokenType::Letter;
    } else {
      return TokenType::Digit;
    }
  }
  return getTokenType(std::to_string(value));
}

std::string getTokenValue(const TokenType &token) {
  auto value = sTokenTypeValueMap.find(token);
  if (value != sTokenTypeValueMap.end()) {
    return value->second;
  } else {
    auto value = sTokenTypeValueCharMap.find(token);
    if (value != sTokenTypeValueCharMap.end()) {
      return std::to_string(value->second);
    }
  }
  return sTokenTypeValueMap.at(TokenType::None);
}
} // namespace Tokens
