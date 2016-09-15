# DZMLang [![Build Status](https://travis-ci.org/zaklaus/DZMLang.svg?branch=master)](https://travis-ci.org/zaklaus/DZMLang) [![Apache 2 licensed](https://img.shields.io/hexpm/l/plug.svg?maxAge=2592000)](https://github.com/zaklaus/DZMLang/blob/master/LICENSE)

Functional language written in C++.

## Contains
* Working interpreter. (Lexer, Evaluator, Printer)
* Contains basic types: Chars, Numbers, Strings, Symbols, Procedures and compounds, Pairs, I/O handles.
* More advanced data types implemented directly in standard library. (sets, queues, tables, classes, ...)
* Support for procedures added via simple API. (add_proc)
* Basic I/O operations. (write-char, read-char, read, write, peek, open, close, ...)
* ~~Garbage Collector. (WIP)~~
* Standard Library. (WIP)
* Networking support (TCP)
* Uses R5RS language specification.
* Compatible with various others scripts written for different interpreters. (TLC, SICP-esque interpreters)

## Build Instructions
* See [How to compile](https://github.com/zaklaus/DZMLang/wiki/How-to-compile) for build instructions.

## Credits
*  **Hal Abelson's, Jerry Sussman's and Julie Sussman's Structure and Interpretation of Computer Programs** (MIT Press, 1984; ISBN 0-262-01077-1; known as SICP) book.

## Copyright
* DZMLang is licensed under the **Apache 2 License**, see LICENSE for more information.
