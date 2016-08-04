# DZMLang [![Build Status](https://travis-ci.org/zaklaus/DZMLang.svg?branch=master)](https://travis-ci.org/zaklaus/DZMLang) [![MIT licensed](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/zaklaus/DZMLang/blob/master/LICENSE)

Functional language written in C++.

## Contains
* Working interpreter. (Lexer, Evaluator, Printer)
* Contains basic types: Chars, Numbers, Strings, Symbols, Procedures and compounds, Pairs, I/O handles.
* Support for procedures added via simple API. (add_proc)
* Basic I/O operations. (write-char, read-char, read, write, peek, open, close, ...)
* Garbage Collector. (WIP)
* Standard Library. (WIP)
* Uses R5RS language specification.
* Compatible with various others scripts written for different interpreters. (TLC, SICP-esque interpreters)

## Build Instructions
* On Windows
   * Install latest version of MinGW GCC compiler
   * Use build.bat to compile project
   * **NOTE**: ctime.exe is a time-tracking utility which measures build times for me. ctime.exe was developed by [Casey Muratori](https://twitter.com/cmuratori), more info about ctime.exe utility can be found here: [Youtube](https://www.youtube.com/watch?v=LdMHyGxfg6U).
* On Linux
   * Make sure you have the latest stable version of GCC compiler installed.
   * Make sure you have the latest version of libreadline6 and libreadline6-dev installed.
   * `make` in project folder.
* On OSX
   * Project should compile fine on Clang and GCC. Unfortunately, I can't provide any assistance with OSX build.

## Credits
*  **Hal Abelson's, Jerry Sussman's and Julie Sussman's Structure and Interpretation of Computer Programs** (MIT Press, 1984; ISBN 0-262-01077-1; known as SICP) book.

## Copyright
* DZMLang is licensed under the **MIT License**, see LICENSE for more information.
