#ifndef AST_LIST_NODE_HPP
#define AST_LIST_NODE_HPP

#include <string>

#include "symbol_table_list_node.hpp"
#include "token_enum.hpp"
#include "token_node.hpp"

enum class ASTNodeType {
    DECLARATION,
    ASSIGNMENT,
    RETURN,
    PRINTF,
    IF,
    ELSE,
    WHILE,
    FOR1,
    FOR2,
    FOR3,
    BEGIN_BLOCK,
    END_BLOCK,
    SIBLING,
    AMBIGUOUS,
    UNKNOWN,
};

static const char* typeToAString(ASTNodeType type) {
    switch (type) {
        case ASTNodeType::DECLARATION:
            return "DECLARATION";
        case ASTNodeType::ASSIGNMENT:
            return "ASSIGNMENT";
        case ASTNodeType::RETURN:
            return "RETURN";
        case ASTNodeType::PRINTF:
            return "PRINTF";
        case ASTNodeType::IF:
            return "IF";
        case ASTNodeType::ELSE:
            return "ELSE";
        case ASTNodeType::WHILE:
            return "WHILE";
        case ASTNodeType::FOR1:
            return "FOR1";
        case ASTNodeType::FOR2:
            return "FOR2";
        case ASTNodeType::FOR3:
            return "FOR3";
        case ASTNodeType::BEGIN_BLOCK:
            return "BEGIN_BLOCK";
        case ASTNodeType::END_BLOCK:
            return "END_BLOCK";
        case ASTNodeType::SIBLING:
            return "SIBLING";
        case ASTNodeType::AMBIGUOUS:
            return "AMBIGUOUS";
        default:
            return "UNKNOWN";
    }
}

// FOR and INDENTIFER are ambiguous, dependent on FOR(1,2,3) and ASSIGNMENT vs call
static const ASTNodeType tokenTypeToASType(TokenType type) {
    switch (type) {
        // declaration
        case TokenType::FUNCTION:
        case TokenType::PROCEDURE:
        case TokenType::INT:
        case TokenType::CHAR:
        case TokenType::BOOL:
            return ASTNodeType::DECLARATION;

        // boolean exp
        case TokenType::IF:
            return ASTNodeType::IF;
        case TokenType::WHILE:
            return ASTNodeType::WHILE;
        case TokenType::ELSE:
            return ASTNodeType::ELSE;

        // other
        case TokenType::RETURN:
            return ASTNodeType::RETURN;
        case TokenType::PRINTF:
            return ASTNodeType::PRINTF;
        case TokenType::FOR:
        case TokenType::IDENTIFIER:
            return ASTNodeType::AMBIGUOUS;

        // blocks
        case TokenType::L_BRACE:
            return ASTNodeType::BEGIN_BLOCK;
        case TokenType::R_BRACE:
            return ASTNodeType::END_BLOCK;
        default:
            return ASTNodeType::UNKNOWN;
    }
}

// class ASTSiblingNode;

class ASTListNode {
   public:
    ASTListNode() = default;
    ASTListNode(ASTNodeType type) : type(type), symbol(nullptr), lexeme(typeToAString(type)), token(nullptr) {};

    SymbolTableListNode* symbol;
    ASTNodeType type;
    std::string lexeme;
    TokenNode* token;

    ASTListNode* sibling;
    ASTListNode* child;
};

// class ASTSiblingNode : public ASTListNode {
//    public:
//     ASTSiblingNode(TokenNode* token) : ASTListNode(ASTNodeType::SIBLING), token(token) {};
//     ASTSiblingNode* sibling;
// };

#endif
