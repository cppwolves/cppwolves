#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include <unordered_map>
#include <variant>
#include <vector>

#include "ast.hpp"
#include "symbol_table.hpp"
#include "token_enum.hpp"

class Executor {
public:
    Executor(ASTree* ast, SymbolTable* symbolTable);
    void execute();

private:
    // Variant type to store different possible values
    using Value = std::variant<int, char, bool, std::string>;

    // Helper functions
    void executeNode(ASTListNode* node);
    Value evaluateExpression(ASTListNode* node);
    Value evaluateBinaryExpression(ASTListNode* node);
    Value evaluateUnaryExpression(ASTListNode* node);
    Value evaluateIdentifier(ASTListNode* node);
    Value evaluateLiteral(ASTListNode* node);

    // Execution functions for different AST node types
    void executeDeclaration(ASTListNode* node);
    void executeAssignment(ASTListNode* node);
    void executeIf(ASTListNode* node);
    void executeWhile(ASTListNode* node);
    void executeFor(ASTListNode* node);
    void executePrintf(ASTListNode* node);
    void executeReturn(ASTListNode* node);
    void executeFunction(ASTListNode* node);
    void executeProcedure(ASTListNode* node);
    void executeBlock(ASTListNode* node);

    // Utility functions
    int getPrecedence(TokenType type);
    Value applyOperator(TokenType type, const Value& left, const Value& right);
    bool isTrue(const Value& value);

    // Symbol table handling
    void setVariable(const std::string& name, const Value& value);
    Value getVariable(const std::string& name);

private:
    ASTree* ast;
    SymbolTable* symbolTable;
    ASTListNode* currentNode;

    // Call stack for function calls, mostly ignore this I am playing around with how to use it properly right now. Any suggestions are welcome.
    std::vector<std::unordered_map<std::string, Value>> callStack;
};

#endif // EXECUTOR_HPP
