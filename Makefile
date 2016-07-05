CC=gcc
CCFLAGS=-g -std=gnu99 -Wall -Wextra -Werror

all:
    $(CC) $(CCFLAGS) src/test.c -o test