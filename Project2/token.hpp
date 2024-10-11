// token.hpp
#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include "token_enum.hpp"

class Token {
public:
    TokenType type;
    std::string lexeme;
    int lineNumber;

    Token(TokenType type, const std::string& lexeme, int lineNumber);

    std::string getTypeName() const;
};

#endif // TOKEN_HPP
