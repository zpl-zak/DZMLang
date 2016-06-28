@echo off

ctime -begin test_build.ctm
clang -std=c99 -Wall test.c -o test.exe
ctime -end test_build.ctm