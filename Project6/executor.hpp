#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include <unordered_map>
#include <variant>
#include <vector>
#include "interpreter.hpp"
#include "ast.hpp"
#include "symbol_table.hpp"
#include "token_enum.hpp"

class Executor {
public:
    Executor(ASTree* ast, SymbolTable* symbolTable, Interpreter interpreter);
    void execute();

private:
    // same variant type that exists in symbol table
    using Value = SymbolTableListNode::Value;

    // Variant type to store different possible values
    //using Value = std::variant<int, char, bool, std::string>;

    // Helper functions
    void executeNode(ASTListNode* node);
    Value evaluateExpression(ASTListNode* node);
    Value evaluateBinaryExpression(ASTListNode* node);
    Value evaluateUnaryExpression(ASTListNode* node);
    Value evaluateIdentifier(ASTListNode* node);
    Value evaluateLiteral(ASTListNode* node);

    // silvia, rename/alter as needed
    Value evaluateNumExpPostfix();
    bool evaluateBooleanExpPostfix();
    bool isNumber(std::string& str);
    std::pair<int, int> getTwoThingsFromStack(std::stack<int>& stack);

    // Execution functions for different AST node types
    void executeDeclaration(ASTListNode* node);
    void executeAssignment(ASTListNode* node);
    void executeIf();
    void executeWhile(ASTListNode* node);
    void executeFor(ASTListNode* node);
    void executePrintf();
    Value executeReturn();
    Value executeFunction();
    void executeProcedure(ASTListNode* node);
    void executeBlock();

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
    Interpreter interpreter;

    // Call stack for function calls, mostly ignore this I am playing around with how to use it properly right now. Any suggestions are welcome.
    std::vector<std::unordered_map<std::string, Value>> callStack;

    // either astnodes or just integers that are used to index the vector of addresses?  I'm not sure
    std::stack<ASTListNode*> programCounter;

    // not sure if this should be nodes or the actual values
    std::stack<ASTListNode> expStack;

};

#endif // EXECUTOR_HPP