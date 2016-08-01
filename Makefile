CC=g++
CCTR=g++-4.8
CCFLAGS=-g -std=gnu++11 -Wall -Wextra -Werror

all:
	$(CC) $(CCFLAGS) src/test.cpp -o test

travis:
	$(CCTR) -g -std=gnu++11 -Wall -Wextra -Werror src/test.cpp -o test