#include "executor.hpp"

#include <iostream>
#include <stack>


Executor::Executor(ASTree* ast, SymbolTable* symbolTable, Interpreter interpreter)
        : ast(ast), symbolTable(symbolTable), currentNode(nullptr), interpreter(interpreter) {

    // currentNode is now pointing at main, could also push this onto a stack if we use one
    currentNode = interpreter.getMain();
}

void Executor::execute() {
    while (currentNode) {
        executeNode(currentNode);

        // all functions should move currentNode node to the last sibling
        // so here we just take the child of currentNode
        currentNode = currentNode->child;
    }
}

void Executor::executeNode(ASTListNode* node) {
    switch (node->type) {
        case ASTNodeType::DECLARATION:
            //executeDeclaration(node);
            break;
        case ASTNodeType::ASSIGNMENT:
            executeAssignment();
            break;
        case ASTNodeType::IF:
            executeIf();
            break;
        case ASTNodeType::WHILE:
            executeWhile();
            break;
        case ASTNodeType::FOR1:
            executeFor();
            break;
        case ASTNodeType::PRINTF:
            executePrintf();
            break;
        case ASTNodeType::RETURN:
            executeReturn();
            break;
        case ASTNodeType::SIBLING:
            //std::cout << "Found sibling token." << std::endl;

            if (symbolTable->find(node->token->lexeme) && symbolTable->find(node->token->lexeme)->identifierType == TokenType::FUNCTION) {
                executeFunction();
            }
            break;
        case ASTNodeType::CALL:
            executeProcedure();
            break;
        case ASTNodeType::BEGIN_BLOCK:
            //callStack.emplace_back();
            break;
        case ASTNodeType::END_BLOCK:
            //callStack.pop_back();
            break;
        default:
            // Handle other node types if necessary
            break;
    }
}

void Executor::executeDeclaration(ASTListNode* node) {
    //std::string varName = node->token->lexeme;
    // Initialize with default value
    //setVariable(varName, 0);
}

void Executor::executeAssignment() {
    // need to move to first node in expression
    currentNode = currentNode->sibling;

    // save current node to update its value
    ASTListNode* nodeToBeAssigned = currentNode;
    currentNode = currentNode->sibling;
    nodeToBeAssigned->symbol->value = evaluateNumExpPostfix();

    // assume for now that curentNode is at assignment operator after
    // evaluateNumExpPostfix, else we need to move currentNode to last sibling here

}

void Executor::executeIf() {
    int bracketCount = 0;

    currentNode = currentNode->sibling;
    bool condition = evaluateBooleanExpPostfix();
    if (condition) {
        executeBlock();

        if (currentNode->child->type == ASTNodeType::ELSE) {
            currentNode = currentNode->child->child;
            //advance past else to first bracket
            bracketCount++;
            while (bracketCount > 0) {
                if (currentNode->sibling) {
                    currentNode = currentNode->sibling;
                }
                else {
                    currentNode = currentNode->child;
                    if (currentNode->type == ASTNodeType::BEGIN_BLOCK) {
                        bracketCount++;
                    }
                    else if (currentNode->type == ASTNodeType::END_BLOCK) {
                        bracketCount--;
                    }
                }
            }
        }
    }
    else {
        //if condition false, need to advance past the if block to the APPROPRIATE end_block
        while (currentNode->type != ASTNodeType::END_BLOCK) {
            if (currentNode->sibling) {
                currentNode = currentNode->sibling;
            }
            else {
                currentNode = currentNode->child;
            }
        }
        //std::cout << "cond false" << std::endl;
        if (currentNode->child->type == ASTNodeType::ELSE) {
            //std::cout << "else exec" << std::endl;
            executeBlock();
            //currentNode = currentNode->child;
        }
    }
}

void Executor::executeWhile() {
    int bracketCount = 0;
    currentNode = currentNode->sibling;
    ASTListNode* conditionNode = currentNode;
    ASTListNode* whileEndNode = nullptr;
    bool condition = evaluateBooleanExpPostfix();

    while (condition) {
        executeBlock();
        whileEndNode = currentNode;
        currentNode = conditionNode;
        condition = evaluateBooleanExpPostfix();
    }

    //once it fails move to end
    if (whileEndNode) {
        currentNode = whileEndNode;
    }
    else {
        //advance past first bracket
        currentNode = currentNode->child;
        bracketCount++;

        while (bracketCount > 0) {
            if (currentNode->sibling) {
                currentNode = currentNode->sibling;
            }
            else {
                currentNode = currentNode->child;
                if (currentNode->type == ASTNodeType::BEGIN_BLOCK) {
                    bracketCount++;
                }
                else if (currentNode->type == ASTNodeType::END_BLOCK) {
                    bracketCount--;
                }
            }
        }
    }

}

void Executor::executeFor() {
    int bracketCount = 0;
    ASTListNode* forEndNode = nullptr;

    // For node is split into FOR1, FOR2, FOR3
    // FOR1: Initialization
    executeAssignment();
    currentNode = currentNode->child;

    // FOR2 end condition
    ASTListNode* conditionNode = currentNode;
    bool condition = evaluateBooleanExpPostfix();
    //eval ends on last node in expression
    currentNode = currentNode->child;

    //FOR3
    ASTListNode* incrementNode = conditionNode->sibling;
    //proceed through to next ast row
    while (currentNode->sibling) {
        currentNode = currentNode->sibling;
    }
    currentNode = currentNode->child;

    //evaluate endCondition
    while (condition) {
        executeBlock();
        forEndNode = currentNode;
        currentNode = incrementNode;
        executeAssignment();
        currentNode = conditionNode;
        condition = evaluateBooleanExpPostfix();
    }

    //once it fails move to end
    if (forEndNode) {
        currentNode = forEndNode;
    }
    else {
        //advance past first bracket
        currentNode = currentNode->child;
        bracketCount++;

        while (bracketCount > 0) {
            if (currentNode->sibling) {
                currentNode = currentNode->sibling;
            }
            else {
                currentNode = currentNode->child;
                if (currentNode->type == ASTNodeType::BEGIN_BLOCK) {
                    bracketCount++;
                }
                else if (currentNode->type == ASTNodeType::END_BLOCK) {
                    bracketCount--;
                }
            }
        }
    }
}

void Executor::executePrintf() {
    currentNode = currentNode->sibling;
    std::string formatString = currentNode->token->lexeme;
    std::vector<Value> args;

    //if there are arguments
    if (currentNode->sibling) {
        currentNode = currentNode->sibling;

        while (currentNode) {
            args.emplace_back(symbolTable->find(currentNode->token->lexeme)->value);

            if (currentNode->sibling == nullptr) {
                break;
            }
            else {
                currentNode = currentNode->sibling;
            }
        }
    }


    int arg_count = 0;
    // For simplicity, assume all arguments are integers
    for (std::string::iterator it = formatString.begin(); it != formatString.end(); it++)
    {
        if (*it == '\\')
        {
            if (*++it ) {
                std::cout << std::endl;
                continue;
            }
            else {
                *--it;
            }
        }

        if (*it != '%')
        {
            putchar(*it);
            continue;
        }
        switch (*++it)
        {
            case 'd':
                printf("%d", std::get<int>(args[arg_count]));
                break;
            case 's':
                printf("%s", std::get<std::string>(args[arg_count]).c_str());
                break;
            default:
                putchar(*it);
                break;
        }

        arg_count++;
    }
}

Executor::Value Executor::executeReturn() {
    Value returnValue = 0;

    if (currentNode->sibling) {
        //should call numPostFixExpression
        currentNode = currentNode->sibling;
        returnValue = evaluateNumExpPostfix();
    }

    //moving currentNode back to caller node position
    currentNode = programCounter.top();
    programCounter.pop();

    return returnValue;
}

Executor::Value Executor::executeFunction() {
    ASTListNode* returnNode;
    ASTListNode* functionNode = interpreter.getAddressAtInd(symbolTable->find(currentNode->token->lexeme)->address);
    SymbolTableListNode* curParamNode = functionNode->symbol->parameterList;
    currentNode = currentNode->sibling;


    // itterate through parameter nodes to assign corresponding values into symbol table
    while(curParamNode) {
        curParamNode->value = symbolTable->find(currentNode->token->lexeme, currentNode->symbol->scope)->value;
        curParamNode = curParamNode->next();
        currentNode = currentNode->sibling;

        // push returnNode onto stack to reset currentNode once function ends
        returnNode = currentNode;
        programCounter.push(returnNode);

    }

    // currentNode is now pointing at the location of the function so that
    // other execution functions will be processing inside the function body
    currentNode = functionNode;

    // loop through function and handle executions
    while (currentNode->type != ASTNodeType::RETURN) {
        executeNode(currentNode);
        if (currentNode->sibling) {
            currentNode = currentNode->sibling;
        }
        else {
            currentNode = currentNode->child;
        }
    }

    return executeReturn();
}

void Executor::executeProcedure() {
    ASTListNode* returnNode;
    ASTListNode* functionNode = interpreter.getAddressAtInd(symbolTable->find(currentNode->token->lexeme)->address);
    SymbolTableListNode* curParamNode = functionNode->symbol->parameterList;
    currentNode = currentNode->sibling;

    // iterate through parameter nodes to assign corresponding values into symbol table
    while(curParamNode) {
        curParamNode->value = symbolTable->find(currentNode->token->lexeme, currentNode->symbol->scope)->value;
        curParamNode = curParamNode->next();

        if (curParamNode) {
            currentNode = currentNode->sibling;
        }
        else {
            // push returnNode onto stack to reset currentNode once function ends
            returnNode = currentNode;
            programCounter.push(returnNode);
        }

    }

    // currentNode is now pointing at the location of the function so that
    // other execution functions will be processing inside the function body
    currentNode = functionNode;

    // loop through function and handle executions
    while (currentNode->type != ASTNodeType::END_BLOCK) {
        executeNode(currentNode);
        if (currentNode->sibling) {
            currentNode = currentNode->sibling;
        }
        else {
            currentNode = currentNode->child;
        }
    }

    currentNode = programCounter.top();
    programCounter.pop();
}

void Executor::executeBlock() {
    //callStack.emplace_back();  // New scope
    //move past begin block
    currentNode = currentNode->child->child;
    while (currentNode->type != ASTNodeType::END_BLOCK) {
        executeNode(currentNode);
        //std::cout << currentNode->lexeme << std::endl;
        if (currentNode->sibling) {
            currentNode = currentNode->sibling;
        }
        else {
            currentNode = currentNode->child;
        }
    }
    //callStack.pop_back();  // End scope
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

// actual execution of this cant be finished until you guys decide how to handle runtime values
// note to jacob: varient types seem to add a lot of weird overhead to handle the possibilites correctly
// i'd reconsider if they're worth it
Executor::Value Executor::evaluateNumExpPostfix() {
    std::stack<int> stack;

    // lazy way to hold first val for postfix exp in assignment statements
    // if assignment operator is never seen, this is irrelevant (in a good way)
    //auto assignmentTarget = node;

    while (currentNode) {
        if (isNumber(currentNode->token->lexeme)) {
            stack.push(std::stoi(currentNode->token->lexeme));
        } else if (currentNode->token->type == TokenType::IDENTIFIER) {
            int varValue;

            // check if identifier is a function call
            // if true, push the function's return value onto the stack as an int
            if (symbolTable->find(currentNode->token->lexeme)->identifierType == TokenType::FUNCTION) {
                varValue = std::get<int>(executeFunction());
            }
            else {
                varValue = std::get<int>(currentNode->symbol->value);
            }

            // push identifier's value onto the stack
            stack.push(varValue);

        } else if (currentNode->token->type == TokenType::ASSIGNMENT_OPERATOR) {
            // assign runtime value of identifier
            // ex: assignmentTarget->symbol->value = stack.top();
            // leaves stack empty, which is maybe fine to assume only bc no assignments inside of statements?
            //stack.pop();

        } else {
            // is operator
            auto [lhs, rhs] = getTwoThingsFromStack(stack);

            switch (currentNode->token->type) {
                case TokenType::PLUS:
                    stack.push(lhs + rhs);
                    break;
                case TokenType::MINUS:
                    stack.push(lhs + rhs);
                    break;
                case TokenType::ASTERISK:
                    stack.push(lhs * rhs);
                    break;
                case TokenType::DIVIDE:
                    stack.push(lhs / rhs);
                    break;
                case TokenType::MODULO:
                    stack.push(lhs % rhs);
                    break;
                default:
                    break;// err
            }
        }

        if (currentNode->sibling == nullptr) {
            break;
        }
        else {
            currentNode = currentNode->sibling;
        }
    }

    int result = stack.top();
    return result;
}

// essentially the same as evaluateNumExpPostfix
// using an int stack bc implicit convertions exists, yay
bool Executor::evaluateBooleanExpPostfix() {
    std::stack<int> stack;

    while (currentNode) {
        if (currentNode->token->type == TokenType::TRUE) {
            stack.push(1);
        } else if (currentNode->token->type == TokenType::FALSE) {
            stack.push(0);
        } else if (isNumber(currentNode->token->lexeme)) {
            stack.push(std::stoi(currentNode->token->lexeme));
        } else if (currentNode->token->type == TokenType::IDENTIFIER) {
            int varValue;

            // check if identifier is a function call
            // if true, push the function's return value onto the stack as an int
            if (symbolTable->find(currentNode->token->lexeme)->identifierType == TokenType::FUNCTION) {
                varValue = std::get<int>(executeFunction());
            }
            else {
                varValue = std::get<int>(currentNode->symbol->value);
            }

            // push identifier's value onto the stack
            stack.push(varValue);

        } else if (currentNode->token->type == TokenType::ASSIGNMENT_OPERATOR) {
            // assign runtime value of identifier
            // ex: assignmentTarget->symbol->value = stack.top();

            //stack.pop();

        } else {
            // is operator
            auto [lhs, rhs] = getTwoThingsFromStack(stack);

            switch (currentNode->token->type) {
                case TokenType::PLUS:
                    stack.push(lhs + rhs);
                    break;
                case TokenType::MINUS:
                    stack.push(lhs + rhs);
                    break;
                case TokenType::ASTERISK:
                    stack.push(lhs * rhs);
                    break;
                case TokenType::DIVIDE:
                    stack.push(lhs / rhs);
                    break;
                case TokenType::MODULO:
                    stack.push(lhs % rhs);
                    break;
                case TokenType::GT:
                    stack.push(lhs > rhs);
                    break;
                case TokenType::GT_EQUAL:
                    stack.push(lhs >= rhs);
                    break;
                case TokenType::LT:
                    stack.push(lhs < rhs);
                    break;
                case TokenType::LT_EQUAL:
                    stack.push(lhs <= rhs);
                    break;
                case TokenType::BOOLEAN_AND:
                    stack.push(lhs && rhs);
                    break;
                case TokenType::BOOLEAN_OR:
                    stack.push(lhs || rhs);
                    break;
                case TokenType::BOOLEAN_EQUAL:
                    stack.push(lhs == rhs);
                    break;
                case TokenType::BOOLEAN_NOT_EQUAL:
                    stack.push(lhs != rhs);
                    break;
                default:
                    break;// err
            }
        }

        if (currentNode->sibling == nullptr) {
            break;
        }
        else {
            currentNode = currentNode->sibling;
        }
    }

    bool result = stack.top();
    return result;
}

bool Executor::isNumber(std::string& str) {
    try {
        std::stoi(str);
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

std::pair<int, int> Executor::getTwoThingsFromStack(std::stack<int>& stack) {
    int rhs = stack.top();
    stack.pop();
    int lhs = stack.top();
    stack.pop();
    return {lhs, rhs};
}