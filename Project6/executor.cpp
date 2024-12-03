#include "executor.hpp"

#include <iostream>
#include <set>
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
            break;
        case ASTNodeType::END_BLOCK:
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
    //check for assignment type?
    nodeToBeAssigned->symbol->value = evaluateExpression();

    // assume for now that curentNode is at assignment operator after
    // evaluateNumExpPostfix, else we need to move currentNode to last sibling here

}

void Executor::executeIf() {
    int bracketCount = 0;

    currentNode = currentNode->sibling;
    bool condition = std::get<bool>(evaluateExpression());
    if (condition) {
        currentNode = currentNode->child->child;
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
            currentNode = currentNode->child->child;
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
    bool condition = std::get<bool>(evaluateExpression());

    while (condition) {
        currentNode = currentNode->child->child;
        executeBlock();
        whileEndNode = currentNode;
        currentNode = conditionNode;
        condition = std::get<bool>(evaluateExpression());
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
    currentNode = currentNode->sibling;
    ASTListNode* conditionNode = currentNode;
    bool condition = std::get<bool>(evaluateExpression());
    //eval ends on last node in expression
    currentNode = currentNode->child;

    //FOR3
    //currentNode = currentNode->sibling;
    ASTListNode* incrementNode = currentNode;
    //proceed through to next ast row w/out incrementing
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
        condition = std::get<bool>(evaluateExpression());
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

    std::string strArg;
    std::string escape = "\\x0";
    size_t posOfStringEnd;
    int arg_count = 0;

    for (std::string::iterator it = formatString.begin(); it != formatString.end(); it++)
    {
        if (*it == '\\')
        {
            if (*++it == 'n') {
                std::cout << std::endl;
                continue;
            }
            else {
                --it;
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
                //printf("%s", std::get<std::string>(args[arg_count]).c_str());
                strArg = std::get<std::string>(args[arg_count]);
                posOfStringEnd = strArg.find(escape);

                if (posOfStringEnd != std::string::npos) {
                    strArg = strArg.substr(0, posOfStringEnd);
                }
                printf("%s", strArg.c_str());
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
        returnValue = evaluateExpression();
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


    // iterate through parameter nodes to assign corresponding values into symbol table
    while(curParamNode) {
        //curParamNode->value = symbolTable->find(currentNode->token->lexeme, currentNode->symbol->scope)->value;
        curParamNode->value = evaluateExpression();
        //problem in evalExpression is no stopping after correct # of args
        curParamNode = curParamNode->next();
        //currentNode = currentNode->sibling;

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

Executor::Value Executor::evaluateExpression() {
    std::set<TokenType> doNothingTokens = {TokenType::ASSIGNMENT_OPERATOR, TokenType::DOUBLE_QUOTE, TokenType::SINGLE_QUOTE};
    std::stack<Value> stack;

    while (currentNode) {
        if (currentNode->token->type == TokenType::STRING) {
            stack.push(currentNode->token->lexeme);
        }
        else if (currentNode->token->type == TokenType::CHAR_LITERAL) {
            stack.push(currentNode->token->lexeme[0]);
        }
        else if (currentNode->token->type == TokenType::INTEGER) {
            stack.push(std::stoi(currentNode->token->lexeme));
        }
        else if (currentNode->token->type == TokenType::TRUE) {
            stack.push(1);
        }
        else if (currentNode->token->type == TokenType::FALSE) {
            stack.push(0);
        }
        else if (currentNode->token->type == TokenType::IDENTIFIER) {
            Value varValue;

            // check if identifier is a function call
            // if true, push the function's return value onto the stack as an int
            if (symbolTable->find(currentNode->token->lexeme)->identifierType == TokenType::FUNCTION) {
                varValue = executeFunction();
            }
            else {
                varValue = currentNode->symbol->value;
            }
            //array access case
            if (currentNode->sibling) {
                if (currentNode->sibling->token->type == TokenType::L_BRACKET) {
                    //std::cout << "found begin of array" << std::endl;
                    ASTListNode* firstIdentifier = currentNode;
                    //advance past l_bracket
                    currentNode = currentNode->sibling->sibling;

                    //eval expr inside brackets
                    int idxValue = std::get<int>(evaluateExpression());
                    // stop at r_bracket

                    varValue = std::get<std::string>((firstIdentifier->symbol->value))[idxValue];
                }
            }

            // push identifier's value onto the stack
            stack.push(varValue);
        }
        else if (doNothingTokens.count(currentNode->token->type)) {

        }
        else if (currentNode->token->type == TokenType::R_BRACKET) {
            //std::cout << "found end of array" << std::endl;
            break;
        }
        else if (isOperator(currentNode->token->type)) {
            // is operator
            auto [lhs_initial, rhs_initial] = getTwoThingsFromStack(stack);
            int lhs, rhs;

            if (lhs_initial.index() == 0) {
                lhs = std::get<int>(lhs_initial);
            }
            if (lhs_initial.index() == 1) {
                char lhs_char = std::get<char>(lhs_initial);
                lhs = lhs_char;
            }
            if (lhs_initial.index() == 2) {
                bool lhs_bool = std::get<bool>(lhs_initial);
                lhs = lhs_bool;
            }
            if (rhs_initial.index() == 0) {
                rhs = std::get<int>(rhs_initial);
            }
            if (lhs_initial.index() == 1) {
                char rhs_char = std::get<char>(rhs_initial);
                rhs = rhs_char;
            }
            if (rhs_initial.index() == 2) {
                bool rhs_bool = std::get<bool>(rhs_initial);
                rhs = rhs_bool;
            }

            switch (currentNode->token->type) {
                case TokenType::PLUS:
                    stack.push(lhs + rhs);
                    break;
                case TokenType::MINUS:
                    stack.push(lhs - rhs);
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

    Value result = stack.top();
    return result;
}


std::pair<Executor::Value, Executor::Value> Executor::getTwoThingsFromStack(std::stack<Value>& stack) {
    Value rhs = stack.top();
    stack.pop();
    Value lhs = stack.top();
    stack.pop();
    return {lhs, rhs};
}