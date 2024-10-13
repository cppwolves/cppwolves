# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17

# Output executable
TARGET = program.exe

# Source files
SRCS = main.cpp token.cpp tokenizer.cpp cst.cpp

# Default target
all: $(TARGET)

# Build the executable
$(TARGET):
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

# Clean rule to remove the executable
clean:
	rm -f $(TARGET)
