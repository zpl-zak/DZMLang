CC=g++
CCFLAGS=-g -std=gnu++11 -Wall -Wextra -Werror
CCLINE=$(CCFLAGS) src/test.cpp -o test
all:
	$(CC) $(CCLINE)