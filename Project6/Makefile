# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -g

# Directories
SRC_DIR = .
OBJ_DIR = obj
INCLUDE_DIR = include  # Optional if you have header files in this directory

# Output executable
TARGET = program.exe

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Rule to build object files from source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)      # Create obj directory if it doesn't exist
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove the executable and object files
clean:
	rm -f $(TARGET)
	rm -rf $(OBJ_DIR)