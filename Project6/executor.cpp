#include "executor.hpp"

#include <iostream>
#include <stack>

Executor::Executor(ASTree* ast, SymbolTable* symbolTable)
    : ast(ast), symbolTable(symbolTable), currentNode(nullptr) {
    // Initialize the call stack with the global scope
    callStack.emplace_back();
}

void Executor::execute() {
    currentNode = ast->head();
    while (currentNode) {
        executeNode(currentNode);
        currentNode = currentNode->child;
    }
}

void Executor::executeNode(ASTListNode* node) {
    switch (node->type) {
        case ASTNodeType::DECLARATION:
            executeDeclaration(node);
            break;
        case ASTNodeType::ASSIGNMENT:
            executeAssignment(node);
            break;
        case ASTNodeType::IF:
            executeIf(node);
            break;
        case ASTNodeType::WHILE:
            executeWhile(node);
            break;
        case ASTNodeType::FOR1:
            executeFor(node);
            break;
        case ASTNodeType::PRINTF:
            executePrintf(node);
            break;
        case ASTNodeType::RETURN:
            executeReturn(node);
            break;
        case ASTNodeType::CALL:
            executeFunction(node);
            break;
        case ASTNodeType::BEGIN_BLOCK:
            callStack.emplace_back();
            break;
        case ASTNodeType::END_BLOCK:
            callStack.pop_back();
            break;
        default:
            // Handle other node types if necessary
            break;
    }
}

void Executor::executeDeclaration(ASTListNode* node) {
    std::string varName = node->token->lexeme;
    // Initialize with default value
    setVariable(varName, 0);
}

void Executor::executeAssignment(ASTListNode* node) {
    std::string varName = node->token->lexeme;
    Value value = evaluateExpression(node->sibling);
    setVariable(varName, value);
}

void Executor::executeIf(ASTListNode* node) {
    Value condition = evaluateExpression(node->sibling);
    if (isTrue(condition)) {
        executeBlock(node->child);
    } else if (node->sibling->sibling && node->sibling->sibling->type == ASTNodeType::ELSE) {
        executeBlock(node->sibling->sibling->child);
    }
}

void Executor::executeWhile(ASTListNode* node) {
    while (isTrue(evaluateExpression(node->sibling))) {
        executeBlock(node->child);
    }
}

void Executor::executeFor(ASTListNode* node) {
    // For node is split into FOR1, FOR2, FOR3
    // FOR1: Initialization
    executeAssignment(node->sibling);

    ASTListNode* conditionNode = node->sibling->sibling;
    ASTListNode* incrementNode = conditionNode->sibling;

    while (isTrue(evaluateExpression(conditionNode->sibling))) {
        executeBlock(node->child);
        executeAssignment(incrementNode->sibling);
    }
}

void Executor::executePrintf(ASTListNode* node) {
    ASTListNode* argNode = node->sibling;
    std::string formatString = std::get<std::string>(evaluateLiteral(argNode));
    argNode = argNode->sibling;

    // For simplicity, assume all arguments are integers
    printf(formatString.c_str(), std::get<int>(evaluateExpression(argNode)),
           std::get<int>(evaluateExpression(argNode->sibling)));
}

void Executor::executeReturn(ASTListNode* node) {
    // Handle return value if necessary
}

void Executor::executeFunction(ASTListNode* node) {
    // Handle function calls
}

void Executor::executeProcedure(ASTListNode* node) {
    // Handle procedures
}

void Executor::executeBlock(ASTListNode* node) {
    callStack.emplace_back(); // New scope
    ASTListNode* childNode = node;
    while (childNode) {
        executeNode(childNode);
        childNode = childNode->child;
    }
    callStack.pop_back(); // End scope
}

Executor::Value Executor::evaluateExpression(ASTListNode* node) {
    if (!node) return 0;

    if (node->type == ASTNodeType::SIBLING) {
        return evaluateExpression(node->sibling);
    }

    if (node->token) {
        switch (node->token->type) {
            case TokenType::INTEGER:
                return std::stoi(node->token->lexeme);
            case TokenType::IDENTIFIER:
                return getVariable(node->token->lexeme);
            case TokenType::PLUS:
            case TokenType::MINUS:
            case TokenType::ASTERISK:
            case TokenType::DIVIDE:
            case TokenType::MODULO:
                return evaluateBinaryExpression(node);
            default:
                break;
        }
    }

    return 0;
}

Executor::Value Executor::evaluateBinaryExpression(ASTListNode* node) {
    Value left = evaluateExpression(node->sibling);
    Value right = evaluateExpression(node->sibling->sibling);
    return applyOperator(node->token->type, left, right);
}

Executor::Value Executor::evaluateUnaryExpression(ASTListNode* node) {
    // Implement unary expression evaluation if necessary
    return 0;
}

Executor::Value Executor::evaluateIdentifier(ASTListNode* node) {
    return getVariable(node->token->lexeme);
}

Executor::Value Executor::evaluateLiteral(ASTListNode* node) {
    if (node->token->type == TokenType::STRING) {
        return node->token->lexeme;
    } else if (node->token->type == TokenType::INTEGER) {
        return std::stoi(node->token->lexeme);
    } else if (node->token->type == TokenType::CHAR_LITERAL) {
        return node->token->lexeme[0];
    } else if (node->token->type == TokenType::TRUE) {
        return true;
    } else if (node->token->type == TokenType::FALSE) {
        return false;
    }
    return 0;
}

int Executor::getPrecedence(TokenType type) {
    // Define operator precedence if necessary
    return 0;
}

Executor::Value Executor::applyOperator(TokenType type, const Value& left, const Value& right) {
    if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
        int lhs = std::get<int>(left);
        int rhs = std::get<int>(right);
        switch (type) {
            case TokenType::PLUS:
                return lhs + rhs;
            case TokenType::MINUS:
                return lhs - rhs;
            case TokenType::ASTERISK:
                return lhs * rhs;
            case TokenType::DIVIDE:
                return lhs / rhs;
            case TokenType::MODULO:
                return lhs % rhs;
            case TokenType::BOOLEAN_EQUAL:
                return lhs == rhs;
            case TokenType::BOOLEAN_NOT_EQUAL:
                return lhs != rhs;
            case TokenType::LT:
                return lhs < rhs;
            case TokenType::GT:
                return lhs > rhs;
            case TokenType::LT_EQUAL:
                return lhs <= rhs;
            case TokenType::GT_EQUAL:
                return lhs >= rhs;
            default:
                break;
        }
    }
    // Handle other types if necessary
    return 0;
}

bool Executor::isTrue(const Value& value) {
    if (std::holds_alternative<int>(value)) {
        return std::get<int>(value) != 0;
    } else if (std::holds_alternative<bool>(value)) {
        return std::get<bool>(value);
    }
    return false;
}

void Executor::setVariable(const std::string& name, const Value& value) {
    if (!callStack.empty()) {
        callStack.back()[name] = value;
    }
}

Executor::Value Executor::getVariable(const std::string& name) {
    for (auto scopeIt = callStack.rbegin(); scopeIt != callStack.rend(); ++scopeIt) {
        auto varIt = scopeIt->find(name);
        if (varIt != scopeIt->end()) {
            return varIt->second;
        }
    }
    // Variable not found; handle error or return default value
    return 0;
}
