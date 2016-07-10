@echo off

ctime -begin test_build.ctm
g++ -g -std=gnu++11 -Wall -Wextra -Werror src\test.cpp -o test.exe
ctime -end test_build.ctm