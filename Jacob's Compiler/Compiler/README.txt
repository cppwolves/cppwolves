Example Usage:

In linux terminal:
   make
   ./program.exe tests_2/programming_assignment_2-test_file_1.c

Result:
    A new file called program_output.txt should appear, containing the tokenizer output.
    Open it, it should match the test case output.

In a windows terminal:
    g++ -std=c++17 -o program.exe main.cpp tokenizer.cpp token.cpp
    ./program.exe tests_2/programming_assignment_2-test_file_1.c

Result:
    Same as linux.


Note that make files are basically exclusive to linux, so tring to use make on a windows machine without
WSL installed results in the terminal returning an unknown command error. If you don't have WSL install it
or simply use the windows command I provided. Just note the windows command line is not robust and wont work
if we add new files to the project.