CC=g++
CCFLAGS=-g -std=gnu++11 -Wall -Wextra -Werror

all:
	$(CC) $(CCFLAGS) src/test.cpp -o test