# DZMLang ![build](https://travis-ci.org/zaklaus/DZMLang.svg)

Scheme-like functional language written in C.

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
   * There is no makefile for automating building process yet. Use gcc build line from build.bat to build your linux executable
* On OSX
   * Project should compile fine on Clang and GCC. Unfortunately, I can't provide any assistance with OSX build.

## Known Bugs
* Interpreter crashes when using GlobalEnv objects inside of evaluated environment "(eval '(arena-mem) environment)".
* GC behaves funny for several cases:
    * printing empty symbols like a,b then printing ab locks up the interpreter,
    * printing the same quote several times will trigger dead loop in write and write_pair,
    * random crashes when mixing input. (ie. writing char, then symbol, calling procedure, ...)
* **(Bugs will be moved later into Issues page)**

## Credits
*  **Hal Abelson's, Jerry Sussman's and Julie Sussman's Structure and Interpretation of Computer Programs** (MIT Press, 1984; ISBN 0-262-01077-1; known as SICP) book.

## Copyright
* DZMLang is licensed under the **MIT License**, see LICENSE for more information.
