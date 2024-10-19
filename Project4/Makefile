# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -g

# Output executable
TARGET = program.exe

# Source files
SRCS = main.cpp token.cpp tokenizer.cpp cst.cpp table.cpp symbol.cpp

# Default target
all: $(TARGET)

# Build the executable
$(TARGET):
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

# Clean rule to remove the executable
clean:
	rm -f $(TARGET)
