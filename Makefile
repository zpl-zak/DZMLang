CC=g++
CCFLAGS=-g -std=gnu++11 -Wall -Wextra -Werror
CCLINE=$(CCFLAGS) src/app.cpp -o dzm
all:
	$(CC) $(CCLINE) -lreadline
win:
	$(CC) $(CCLINE)
clean:
	rm dzm
	rm dzm_log.txt

