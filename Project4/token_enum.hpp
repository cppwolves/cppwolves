// token_enum.hpp
#ifndef TOKEN_ENUM_HPP
#define TOKEN_ENUM_HPP

enum class TokenType {
  // Keywords
  FUNCTION,
  PROCEDURE,
  IF,
  ELSE,
  FOR,
  WHILE,
  RETURN,
  CHAR,
  INT,
  BOOL,
  TRUE,
  FALSE,
  VOID,
  MAIN,
  PRINTF,
  GETCHAR,

  // Identifiers
  IDENTIFIER,

  // Literals
  INTEGER,
  STRING,
  CHAR_LITERAL,

  // Operators
  ASSIGNMENT_OPERATOR,
  PLUS,
  MINUS,
  ASTERISK,
  DIVIDE,
  MODULO,
  CARET,
  LT,
  GT,
  LT_EQUAL,
  GT_EQUAL,
  BOOLEAN_AND,
  BOOLEAN_OR,
  BOOLEAN_NOT,
  BOOLEAN_EQUAL,
  BOOLEAN_NOT_EQUAL,

  // Delimiters
  L_PAREN,
  R_PAREN,
  L_BRACKET,
  R_BRACKET,
  L_BRACE,
  R_BRACE,
  SEMICOLON,
  COMMA,
  SINGLE_QUOTE,
  DOUBLE_QUOTE,

  // Comments and whitespace (for internal use)
  COMMENT,
  WHITESPACE,
  NOT_APPLICABLE,

  // End of file
  END_OF_FILE,

  // Error
  INVALID_TOKEN
};

static bool isDelimiter(TokenType type) {
  return type == TokenType::L_PAREN || type == TokenType::R_PAREN ||
         type == TokenType::L_BRACKET || type == TokenType::R_BRACKET ||
         type == TokenType::L_BRACE || type == TokenType::R_BRACE ||
         type == TokenType::SEMICOLON || type == TokenType::COMMA ||
         type == TokenType::SINGLE_QUOTE || type == TokenType::DOUBLE_QUOTE;
}

static bool isKeyword(TokenType type) {
  return type == TokenType::FUNCTION || type == TokenType::PROCEDURE ||
         type == TokenType::IF || type == TokenType::ELSE ||
         type == TokenType::FOR || type == TokenType::WHILE ||
         type == TokenType::RETURN || type == TokenType::CHAR ||
         type == TokenType::INT || type == TokenType::BOOL ||
         type == TokenType::TRUE || type == TokenType::FALSE ||
         type == TokenType::VOID || type == TokenType::MAIN ||
         type == TokenType::PRINTF || type == TokenType::GETCHAR;
}

static bool isBooleanOperator(TokenType type) {
  return type == TokenType::BOOLEAN_AND || type == TokenType::BOOLEAN_OR ||
         type == TokenType::BOOLEAN_NOT || type == TokenType::BOOLEAN_EQUAL ||
         type == TokenType::BOOLEAN_NOT_EQUAL;
}

static bool isRelationalOperator(TokenType type) {
  return type == TokenType::LT || type == TokenType::GT ||
         type == TokenType::LT_EQUAL || type == TokenType::GT_EQUAL ||
         type == TokenType::BOOLEAN_EQUAL ||
         type == TokenType::BOOLEAN_NOT_EQUAL;
}

static bool isOperator(TokenType type) {
  return type == TokenType::ASSIGNMENT_OPERATOR || type == TokenType::PLUS ||
         type == TokenType::MINUS || type == TokenType::ASTERISK ||
         type == TokenType::DIVIDE || type == TokenType::MODULO ||
         type == TokenType::CARET || isRelationalOperator(type) ||
         isBooleanOperator(type);
}

static bool isNumericOperator(TokenType type) {
  return type == TokenType::PLUS || type == TokenType::CARET ||
         type == TokenType::MINUS || type == TokenType::ASTERISK ||
         type == TokenType::DIVIDE || type == TokenType::MODULO;
}

static bool isDataType(TokenType type) {
  return type == TokenType::CHAR || type == TokenType::INT ||
         type == TokenType::BOOL;
}

static bool isIdentifier(TokenType type) {
  return type == TokenType::FUNCTION || type == TokenType::PROCEDURE ||
         type == TokenType::IF || type == TokenType::ELSE ||
         type == TokenType::FOR || type == TokenType::WHILE ||
         type == TokenType::RETURN || type == TokenType::CHAR ||
         type == TokenType::INT || type == TokenType::BOOL ||
         type == TokenType::TRUE || type == TokenType::FALSE ||
         type == TokenType::VOID || type == TokenType::MAIN ||
         type == TokenType::PRINTF || type == TokenType::GETCHAR ||
         type == TokenType::IDENTIFIER;
}

static const char * typeToCString(TokenType type) {
  switch (type) {
    case TokenType::FUNCTION: return "FUNCTION";
    case TokenType::PROCEDURE: return "PROCEDURE";
    case TokenType::IF: return "IF";
    case TokenType::ELSE: return "ELSE";
    case TokenType::FOR: return "FOR";
    case TokenType::WHILE: return "WHILE";
    case TokenType::RETURN: return "RETURN";
    case TokenType::CHAR: return "CHAR";
    case TokenType::INT: return "INT";
    case TokenType::BOOL: return "BOOL";
    case TokenType::TRUE: return "TRUE";
    case TokenType::FALSE: return "FALSE";
    case TokenType::VOID: return "VOID";
    case TokenType::MAIN: return "MAIN";
    case TokenType::PRINTF: return "PRINTF";
    case TokenType::GETCHAR: return "GETCHAR";

    case TokenType::IDENTIFIER: return "IDENTIFIER";
    case TokenType::INTEGER: return "INTEGER";
    case TokenType::STRING: return "STRING";
    case TokenType::CHAR_LITERAL: return "STRING";

    case TokenType::ASSIGNMENT_OPERATOR: return "ASSIGNMENT_OPERATOR";
    case TokenType::PLUS: return "PLUS";
    case TokenType::MINUS: return "MINUS";
    case TokenType::ASTERISK: return "ASTERISK";
    case TokenType::DIVIDE: return "DIVIDE";
    case TokenType::MODULO: return "MODULO";
    case TokenType::CARET: return "CARET";
    case TokenType::LT: return "LT";
    case TokenType::GT: return "GT";
    case TokenType::LT_EQUAL: return "LT_EQUAL";
    case TokenType::GT_EQUAL: return "GT_EQUAL";
    case TokenType::BOOLEAN_AND: return "BOOLEAN_AND";
    case TokenType::BOOLEAN_OR: return "BOOLEAN_OR";
    case TokenType::BOOLEAN_NOT: return "BOOLEAN_NOT";
    case TokenType::BOOLEAN_EQUAL: return "BOOLEAN_EQUAL";
    case TokenType::BOOLEAN_NOT_EQUAL: return "BOOLEAN_NOT_EQUAL";

    case TokenType::L_PAREN: return "L_PAREN";
    case TokenType::R_PAREN: return "R_PAREN";
    case TokenType::L_BRACKET: return "L_BRACKET";
    case TokenType::R_BRACKET: return "R_BRACKET";
    case TokenType::L_BRACE: return "L_BRACE";
    case TokenType::R_BRACE: return "R_BRACE";
    case TokenType::SEMICOLON: return "SEMICOLON";
    case TokenType::COMMA: return "COMMA";
    case TokenType::SINGLE_QUOTE: return "SINGLE_QUOTE";
    case TokenType::DOUBLE_QUOTE: return "DOUBLE_QUOTE";

    case TokenType::END_OF_FILE: return "END_OF_FILE";
    case TokenType::INVALID_TOKEN: return "INVALID_TOKEN";
    case TokenType::NOT_APPLICABLE: return "NOT_APPLICABLE";
    default: return "UNKNOWN";
  }
}
#endif // TOKEN_ENUM_HPP
