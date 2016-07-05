@echo off

ctime -begin test_build.ctm
gcc -g -std=gnu99 -Wall -Wextra -Werror src\test.c -o test.exe
ctime -end test_build.ctm