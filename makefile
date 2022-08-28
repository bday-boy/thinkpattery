# the compiler: gcc for C program, define as g++ for C++
CC=gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#  -O4   the highest level of compiler opimizations
CFLAGS=-lm -Wall -O4

# the build target executable:
TARGET=thinkpattery

default: $(TARGET)

$(TARGET): ./src/*.c
	$(CC) $(CFLAGS) ./src/*.c -o ./$(TARGET)

clean:
	$(RM) $(TARGET)