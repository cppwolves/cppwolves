// tokenizer.hpp
#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <vector>
#include <fstream>
#include <queue>
#include "token.hpp"

class Tokenizer {
public:
    Tokenizer(const std::string& filename);
    std::vector<Token> tokenize();
    std::string errorMessage;
    std::queue<Token> tokenQueue;

private:
    std::ifstream inputFile;
    int lineNumber;
    char currentChar;
    bool endOfFile;

    // Helper methods
    void advance();
    void skipWhitespace();
    void skipComment();

    Token getNextToken();
    Token identifierOrKeyword();
    Token number();
    Token stringLiteral();
    Token charLiteral();
    Token operatorOrDelimiter();

    bool isKeyword(const std::string& str);
    bool isLetter(char ch);
    bool isDigit(char ch);
    bool isHexDigit(char ch);
    bool isOperatorChar(char ch);
};

#endif // TOKENIZER_HPP
