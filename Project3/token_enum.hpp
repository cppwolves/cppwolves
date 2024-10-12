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

#endif // TOKEN_ENUM_HPP
