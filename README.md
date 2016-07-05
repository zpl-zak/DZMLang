# DZMLang
Scheme-like functional language written in C.

Contains:
* Working interpreter. (Lexer, Evaluator, Printer)
* Contains basic types: Chars, Numbers, Strings, Symbols, Procedures and compounds, Pairs, I/O handles.
* Support for procedures added via simple API. (add_proc)
* Garbage Collector. (WIP)
* Standard Library. (WIP)
* Uses R5RS language specification.
* Compatible with various others scripts written for different interpreters. (TLC, SICP-esque interpreters)

Known Bugs:
* Interpreter crashes when using GlobalEnv objects inside of evaluated environment "(eval '(arena-mem) environment)".
* GC behaves funny for several cases:
* * printing empty symbols like a,b then printing ab locks up the interpreter,
* * printing the same quote several times will trigger dead loop in write and write_pair,
* * random crashes when mixing input. (ie. writing char, then symbol, calling procedure, ...)
* (Bugs will be moved later into Issues page)

Credits:
* Hal Abelson's and Julie Sussman's Structure and Interpretation of Computer Programs (SICP) book.

Copyright:
* DZMLang is licensed under the MIT License, see LICENSE for more information.