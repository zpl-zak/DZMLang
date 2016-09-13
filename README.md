# DZMLang [![Build Status](https://travis-ci.org/zaklaus/DZMLang.svg?branch=master)](https://travis-ci.org/zaklaus/DZMLang) [![MIT licensed](https://img.shields.io/hexpm/l/plug.svg?maxAge=2592000)](https://github.com/zaklaus/DZMLang/blob/master/LICENSE)

Functional language written in C++.

## Contains
* Working interpreter. (Lexer, Evaluator, Printer)
* Contains basic types: Chars, Numbers, Strings, Symbols, Procedures and compounds, Pairs, I/O handles.
* Support for procedures added via simple API. (add_proc)
* Basic I/O operations. (write-char, read-char, read, write, peek, open, close, ...)
* ~~Garbage Collector. (WIP)~~
* Standard Library. (WIP)
* Uses R5RS language specification.
* Compatible with various others scripts written for different interpreters. (TLC, SICP-esque interpreters)

## Build Instructions
* On Windows
   * Windows is not officially supported yet. Check issue #31.
* On Linux and Mac OS X
   * Make sure you have the latest stable version of G++/clang++ compiler installed.
   * Make sure you have the latest version of **libreadline6** and **libreadline6-dev** installed (if applicable).
   * `mkdir build` and `cd build` into it.
   * `cmake ../` inside of the directory
   * `make` the project.

## Credits
*  **Hal Abelson's, Jerry Sussman's and Julie Sussman's Structure and Interpretation of Computer Programs** (MIT Press, 1984; ISBN 0-262-01077-1; known as SICP) book.
* **NOTE**: ctime is a time-tracking utility which measures build times. ctime was developed by [Casey Muratori](https://twitter.com/cmuratori), more info about ctime utility can be found here: [Youtube](https://www.youtube.com/watch?v=LdMHyGxfg6U). This project is using modified Linux version by [mmozeiko](https://github.com/mmozeiko).

## Copyright
* DZMLang is licensed under the **Apache 2 License**, see LICENSE for more information.
