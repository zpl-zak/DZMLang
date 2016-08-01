CC=g++
CCTR=g++-4.9
CCFLAGS=-g -std=gnu++11 -Wall -Wextra -Werror
CCLINE=$(CCFLAGS) src/test.cpp -o test
all:
	$(CC) $(CCLINE)

travis:
	$(CCTR) $(CCLINE)