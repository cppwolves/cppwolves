// main.cpp
#include <cctype>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <stdexcept>

#include "ast.hpp"
#include "cst.hpp"
#include "interpreter.hpp"
#include "symbol_table.hpp"
#include "token_enum.hpp"
#include "token_node.hpp"
#include "tokenizer.hpp"

std::string toLower(const std::string &s) {
    std::string res{};
    for (const char &c : s) {
        res += std::tolower(c);
    }
    return res;
}

std::string boolToYesNo(bool b) { return b ? "yes" : "no"; }

void writeTokens(std::vector<Token> &tokens, const std::string &filename) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        throw std::runtime_error("Error: Could not open output file '" + filename +
                                 "'\n");
    }

    const int labelWidth = 11;   // Width for the label column
    const int valueIndent = 11;  // Indentation for the value column
    outputFile << "Token list:\n\n";
    for (const Token &token : tokens) {
        // Output the Token type
        outputFile << std::left << std::setw(labelWidth) << "Token type:";

        // Align the value
        outputFile << std::setw(valueIndent - labelWidth) << " "
                   << token.getTypeName() << "\n";

        // Output the Token lexeme if available
        if (!token.lexeme.empty()) {
            outputFile << std::left << std::setw(labelWidth) << "Token:";

            // Align the value
            outputFile << std::setw(valueIndent - labelWidth) << " " << token.lexeme
                       << "\n\n";
        }
    }
    std::cout << "Output saved to '" << filename << "'\n";
    outputFile.close();
}

void writeCST(CSTree &cst, const std::string &filename) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        throw std::runtime_error("Error: Could not open output file '" + filename +
                                 "'\n");
    }

    int rowLength = 0;
    TokenNode *node = cst.head();

    while (node) {
        // Output the token lexeme
        std::string str = node->lexeme + " -> ";
        outputFile << str;

        // Update the row length
        rowLength += str.size();

        // If there is a sibling, continue.
        if (node->sibling) {
            node = node->sibling;
            continue;
        }

        // Otherwise, print null and start a new row
        outputFile << "NULL\n";

        // Subtract the current lexeme length to align the down pointer
        rowLength -= node->lexeme.size() + 2;

        // Output the down pointer, adding a new row for each component
        outputFile << std::setw(rowLength) << std::right << "|\n";
        outputFile << std::setw(rowLength) << std::right << "v\n"
                   << std::flush;

        // Pad the next row with spaces to align the next token output
        for (int ws = 0; ws < rowLength - 2; ++ws) {
            outputFile << " ";
        }

        // Align the next output
        rowLength -= 2;
        std::cout << std::flush;

        // If there is no child, output NULL
        if (!node->child) {
            outputFile << "NULL\n";
        }

        // Get the child and continue
        node = node->child;
    }
    std::cout << "Output saved to '" << filename << "'\n";
    outputFile.close();
}

void writeSymbolTable(SymbolTable &st, const std::string &filename) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        throw std::runtime_error("Error: Could not open output file '" + filename +
                                 "'\n");
    }

    const int labelWidth = 20;   // Width for the label column
    const int valueIndent = 20;  // Indentation for the value column

    SymbolTableListNode *symbol = st.head();
    while (symbol) {
        outputFile << std::right << std::setw(labelWidth) << "IDENTIFIER_NAME:";
        outputFile << std::setw(valueIndent - labelWidth) << " "
                   << symbol->identifierName << "\n";

        outputFile << std::right << std::setw(labelWidth) << "IDENTIFIER_TYPE:";
        outputFile << std::setw(valueIndent - labelWidth) << " "
                   << toLower(std::string(typeToCString(symbol->identifierType)))
                   << "\n";

        outputFile << std::right << std::setw(labelWidth) << "DATATYPE:";
        outputFile << std::setw(valueIndent - labelWidth) << " ";
        if (symbol->datatype == TokenType::NOT_APPLICABLE) {
            outputFile << "NOT APPLICABLE\n";
        } else {
            outputFile << toLower(std::string(typeToCString(symbol->datatype)))
                       << "\n";
        }

        outputFile << std::right << std::setw(labelWidth) << "DATATYPE_IS_ARRAY:";
        outputFile << std::setw(valueIndent - labelWidth) << " "
                   << boolToYesNo(symbol->isArray) << "\n";

        outputFile << std::right << std::setw(labelWidth) << "DATATYPE_ARRAY_SIZE:";
        outputFile << std::setw(valueIndent - labelWidth) << " "
                   << symbol->arraySize << "\n";

        outputFile << std::right << std::setw(labelWidth) << "SCOPE:";
        outputFile << std::setw(valueIndent - labelWidth) << " " << symbol->scope
                   << "\n\n";

        symbol = symbol->next();
    }

    symbol = st.head();
    while (symbol) {
        SymbolTableListNode *param = symbol->parameterList;
        if (param) {
            outputFile << std::right << std::setw(labelWidth)
                       << "PARAMETER LIST FOR:";
            outputFile << std::setw(valueIndent - labelWidth) << " "
                       << symbol->identifierName << "\n";

            while (param) {
                outputFile << std::right << std::setw(labelWidth) << "IDENTIFIER_NAME:";
                outputFile << std::setw(valueIndent - labelWidth) << " "
                           << param->identifierName << "\n";

                outputFile << std::right << std::setw(labelWidth) << "DATATYPE:";
                outputFile << std::setw(valueIndent - labelWidth) << " "
                           << toLower(std::string(typeToCString(param->datatype)))
                           << "\n";

                outputFile << std::right << std::setw(labelWidth)
                           << "DATATYPE_IS_ARRAY:";
                outputFile << std::setw(valueIndent - labelWidth) << " "
                           << boolToYesNo(param->isArray) << "\n";

                outputFile << std::right << std::setw(labelWidth)
                           << "DATATYPE_ARRAY_SIZE:";
                outputFile << std::setw(valueIndent - labelWidth) << " "
                           << param->arraySize << "\n";

                outputFile << std::right << std::setw(labelWidth) << "SCOPE:";
                outputFile << std::setw(valueIndent - labelWidth) << " " << param->scope
                           << "\n\n";

                param = param->next();
            }
        }
        symbol = symbol->next();
    }

    std::cout << "Output saved to '" << filename << "'\n";
    outputFile.close();
}

void writeAST(ASTree &ast, const std::string &filename) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        throw std::runtime_error("Error: Could not open output file '" + filename +
                                 "'\n");
    }

    int rowLength = 0;
    // std::cout << "here";
    ASTListNode *node = ast.head();

    while (node) {
        // Output the token lexeme
        std::string str;
        if (node->type == ASTNodeType::SIBLING) {
            str = node->token->lexeme + " -> ";
        } else {
            str = node->lexeme + " -> ";
        }

        // std::string str = node->lexeme + " -> ";
        outputFile << str;

        // Update the row length
        rowLength += str.size();

        // If there is a sibling, continue.
        if (node->sibling) {
            node = node->sibling;
            continue;
        }

        // Otherwise, print null and start a new row
        outputFile << "NULL\n";

        // Subtract the current lexeme length to align the down pointer

        rowLength -= node->type == ASTNodeType::SIBLING ? node->token->lexeme.size() : node->lexeme.size() + 2;

        // Output the down pointer, adding a new row for each component
        outputFile << std::setw(rowLength) << std::right << "|\n";
        outputFile << std::setw(rowLength) << std::right << "v\n"
                   << std::flush;

        // Pad the next row with spaces to align the next token output
        for (int ws = 0; ws < rowLength - 2; ++ws) {
            outputFile << " ";
        }

        // Align the next output
        rowLength -= 2;
        std::cout << std::flush;

        // If there is no child, output NULL
        if (!node->child) {
            outputFile << "NULL\n";
        }

        // Get the child and continue
        node = node->child;
    }
    std::cout << "Output saved to '" << filename << "'\n";
    outputFile.close();
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: tokenizer <input_file>\n";
        return 1;
    }

    try {
        Tokenizer tokenizer(argv[1]);
        std::vector<Token> tokens = tokenizer.tokenize();

        if (!tokenizer.errorMessage.empty()) {
            std::cerr << tokenizer.errorMessage << "\n";
        } else {
            writeTokens(tokens, "tokens_output.txt");

            CSTree tree(tokens);
            writeCST(tree, "cst_output.txt");

            SymbolTable symbolTable(tree);
            writeSymbolTable(symbolTable, "symbol_table_output.txt");

            ASTree aTree(&tree, &symbolTable);
            writeAST(aTree, "ast_output.txt");

            Interpreter interpreter(&aTree, &symbolTable);
        }
    } catch (const std::exception &ex) {
        std::cerr << ex.what() << "\n";
        return 1;
    }

    return 0;
}
