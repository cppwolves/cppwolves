// main.cpp
#include <iostream>
#include <fstream>
#include <iomanip>
#include "tokenizer.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: tokenizer <input_file>\n";
        return 1;
    }

    try {
        Tokenizer tokenizer(argv[1]);
        std::vector<Token> tokens = tokenizer.tokenize();

        // Open the output file
        std::ofstream outputFile("program_output.txt");
        if (!outputFile.is_open()) {
            std::cerr << "Error: Could not open output file 'program_output.txt'\n";
            return 1;
        }

        const int labelWidth = 11;    // Width for the label column
        const int valueIndent = 11;   // Indentation for the value column

        if (!tokenizer.errorMessage.empty()) {
            // An error occurred during tokenization; output only the error message
            outputFile << tokenizer.errorMessage;
            std::cout << tokenizer.errorMessage;
        } else {
            // Output the token list header
            outputFile << "Token list:\n\n";
            for (const Token& token : tokens) {
                // Output the Token type
                outputFile << std::left << std::setw(labelWidth) << "Token type:";

                // Align the value
                outputFile << std::setw(valueIndent - labelWidth) << " "
                        << token.getTypeName() << "\n";

                // Output the Token lexeme if available
                if (!token.lexeme.empty()) {
                    outputFile << std::left << std::setw(labelWidth) << "Token:";

                    // Align the value
                    outputFile << std::setw(valueIndent - labelWidth) << " "
                            << token.lexeme << "\n\n";
                }
            }
            std::cout << "Tokenization complete. Output saved to 'program_output.txt'\n";
        }

        outputFile.close();
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << "\n";
        return 1;
    }

    return 0;
}
