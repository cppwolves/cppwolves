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
        default:
            return "UNKNOWN";
    }
}

class ASTListNode {
   public:
    ASTListNode() = default;
    ASTListNode(ASTNodeType type) : type(type), symbol(nullptr), lexeme(typeToAString(type)) {};

    SymbolTableListNode* symbol;
    ASTNodeType type;
    std::string lexeme;

    ASTListNode* sibling;
    ASTListNode* child;
};

#endif
