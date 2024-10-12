// token.cpp
#include "token.hpp"

Token::Token(TokenType type, const std::string& lexeme, int lineNumber)
    : type(type), lexeme(lexeme), lineNumber(lineNumber) {}

Token::Token(const Token& token)
    : type(token.type), lexeme(token.lexeme), lineNumber(token.lineNumber) {}

std::string Token::getTypeName() const {
    switch (type) {
        case TokenType::FUNCTION: return "IDENTIFIER";
        case TokenType::PROCEDURE: return "IDENTIFIER";
        case TokenType::IF: return "IDENTIFIER";
        case TokenType::ELSE: return "IDENTIFIER";
        case TokenType::FOR: return "IDENTIFIER";
        case TokenType::WHILE: return "IDENTIFIER";
        case TokenType::RETURN: return "IDENTIFIER";
        case TokenType::CHAR: return "IDENTIFIER";
        case TokenType::INT: return "IDENTIFIER";
        case TokenType::BOOL: return "IDENTIFIER";
        case TokenType::TRUE: return "IDENTIFIER";
        case TokenType::FALSE: return "IDENTIFIER";
        case TokenType::VOID: return "IDENTIFIER";
        case TokenType::MAIN: return "IDENTIFIER";
        case TokenType::PRINTF: return "IDENTIFIER";
        case TokenType::GETCHAR: return "IDENTIFIER";

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
        default: return "UNKNOWN";
    }
}
