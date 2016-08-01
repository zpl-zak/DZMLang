CCFLAGS=-g -std=gnu++11 -Wall -Wextra -Werror

all:
	$(CXX) $(CCFLAGS) src/test.cpp -o test