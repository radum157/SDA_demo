CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -O0 -g

# Executable name
TARGET = main

# Source files
C_SRCS = priority_queue.c
CXX_SRCS = main.cpp

# Object files
C_OBJS = $(C_SRCS:.c=.o)
CXX_OBJS = $(CXX_SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Compile C source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

# Link the executable
$(TARGET): $(C_OBJS) $(CXX_OBJS)
	$(CXX) $(C_OBJS) $(CXX_OBJS) -o $(TARGET)

# Clean up
clean:
	rm -f $(C_OBJS) $(CXX_OBJS) $(TARGET)

# Run the benchmark
run: $(TARGET)
	./$(TARGET)

