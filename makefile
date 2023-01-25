# the compiler: gcc for C program, define as g++ for C++
CC=gcc

# compiler flags:
#  -lm   	links to some necessary math libraries
#  -Wall 	turns on most, but not all, compiler warnings
#  -O4   	the highest level of compiler opimizations
CFLAGS=-lm -Wall -O4

# the build target executable:
TARGET=thinkpattery

default: $(TARGET)

$(TARGET): ./src/*.c
	$(CC) $(CFLAGS) ./src/*.c -o ./$(TARGET)

clean:
	$(RM) $(TARGET)