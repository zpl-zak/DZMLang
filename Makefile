CC=g++
CCFLAGS=-g -std=gnu++11 -Wall -Wextra -Werror
CCLINE=$(CCFLAGS) app.cpp -o dzm
CCLIBS=-lm -lpthread
all:
	$(CC) $(CCLINE) -lreadline $(CCLIBS)
win:
	$(CC) $(CCLINE) $(CCLIBS)
clean:
	rm dzm
	rm dzm_log.txt

