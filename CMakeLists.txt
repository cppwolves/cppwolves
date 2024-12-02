cmake_minimum_required(VERSION 3.26)
project(CS460_Project6)

set(CMAKE_CXX_STANDARD 17)

include_directories(Project6)

add_executable(CS460_Project6
        Project6/main.cpp
        Project6/cst.cpp
        Project6/ast.cpp
        Project6/list.cpp
        Project6/symbol_table_list_node.cpp
        Project6/token.cpp
        Project6/symbol_table.cpp
        Project6/tokenizer.cpp
        Project6/list_node.cpp
        Project6/Interpreter.cpp
        Project6/executor.cpp)
