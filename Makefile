# Define the compiler
CXX = g++

# Define the output executable name
TARGET = sudoku_solver

# Define the source file
SRC = sudoku_solver.cpp

# Define the object files
OBJ = $(SRC:.cpp=.o)

# Compiler flags (optional, for debugging and optimizations)
CXXFLAGS = -Wall -g

# The default rule to build the target executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# Rule to create object files from source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Clean rule to remove generated files
clean:
	rm -f $(TARGET) $(OBJ)

# Rule to run the program
run: $(TARGET)
	./$(TARGET)
