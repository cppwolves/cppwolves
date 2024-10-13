// main.cpp
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>

#include "cst.hpp"
#include "token_enum.hpp"
#include "token_node.hpp"
#include "tokenizer.hpp"

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

TokenNode *buildCST(const std::vector<Token> &tokens) {
    if (tokens.empty()) {
        return nullptr;
    }

    TokenNode *head = new TokenNode(tokens.at(0));
    TokenNode *curr = head;

    for (size_t i = 1; i < tokens.size(); ++i) {
        TokenNode *next = new TokenNode(tokens.at(i));
        if (
            // The next token is a delimiter
            (next->type == TokenType::L_BRACE || next->type == TokenType::R_BRACE || next->type == TokenType::ELSE) ||
            // The current token is a delimiter
            (curr->type == TokenType::ELSE || curr->type == TokenType::L_BRACE || curr->type == TokenType::SEMICOLON)) {
            // Start a new row
            curr->child = next;
        } else {
            // Append to current row
            curr->sibling = next;
        }
        curr = next;
    }

    return head;
}

void writeCST(TokenNode *cst, const std::string &filename) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        throw std::runtime_error("Error: Could not open output file '" + filename +
                                 "'\n");
    }

    int rowLength = 0;
    TokenNode *node = cst;

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

            // TokenNode *cst = buildCST(tokens);

            CSTree *tree = new CSTree(tokens);

            if (!tree) {
                std::cout << "CST is empty\n";
            } else {
                writeCST(tree->head(), "cst_output.txt");
            }
        }
    } catch (const std::exception &ex) {
        std::cerr << ex.what() << "\n";
        return 1;
    }

    return 0;
}
