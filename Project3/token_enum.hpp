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

#endif  // TOKEN_ENUM_HPP
