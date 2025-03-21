CC = g++
CXX = g++
CFLAGS = -Wall -Wextra -O0 -g
CXXFLAGS = -Wall -Wextra -O0 -g -std=c++23
LDFLAGS = 

# Executable name
TARGET = benchmark

# Source files
C_SRCS = hashmap.c
CXX_SRCS = benchmark.cpp

# Object files
C_OBJS = $(C_SRCS:.c=.o)
CXX_OBJS = $(CXX_SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Compile C source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile C++ source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link the executable
$(TARGET): $(C_OBJS) $(CXX_OBJS)
	$(CXX) $(CXXFLAGS) $(C_OBJS) $(CXX_OBJS) -o $(TARGET) $(LDFLAGS)

# Clean up
clean:
	rm -f $(C_OBJS) $(CXX_OBJS) $(TARGET)

# Run the benchmark
run: $(TARGET)
	./$(TARGET)
