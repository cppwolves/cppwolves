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
    Value evaluateExpression();
    std::pair<Value, Value> getTwoThingsFromStack(std::stack<Value>& stack);

    // Execution functions for different AST node types
    void executeDeclaration(ASTListNode* node);
    void executeAssignment();
    void executeIf();
    void executeWhile();
    void executeFor();
    void executePrintf();
    Value executeReturn();
    Value executeFunction();
    void executeProcedure();
    void executeBlock();

    // Utility functions
    bool isTrue(const Value& value);

private:
    ASTree* ast;
    SymbolTable* symbolTable;
    ASTListNode* currentNode;
    Interpreter interpreter;

    // either astnodes or just integers that are used to index the vector of addresses?  I'm not sure
    std::stack<ASTListNode*> programCounter;

    // not sure if this should be nodes or the actual values
    std::stack<ASTListNode> expStack;
};

#endif // EXECUTOR_HPP