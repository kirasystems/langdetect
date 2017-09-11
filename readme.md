langdetect
==========

This is a C++ port of the language detection library [languge-detection](https://code.google.com/p/language-detection/). The original port is 
by Hiroyuki Tanaka (https://github.com/aflc/langdetect).

Supported Compiler
==================

GCC >= 4.4 and clang >= 3.1

Install
=======

```
./configure && make install
```

The library and command line application will be installed to `/usr/local` by 
default.

Usage
=====

If you want to use it with your program, see `src/command.cpp`. It's very simple example.

If you want to use it as command line interface, try

```
langdetect path_1, path_2, ...
```

If you want to use it with your C program, include `langdetect_c.h` instead of `langdetect.h`
It has just 2 API, `langdetect_detect` and `langdetect_detect_with_score`, see `src/langdetect_c.h`.

Enjoy!!

Models
======
The models are embedding into the library. The file `src/const.cpp` contains
the models and `scripts/profile_to_code.py` can convert new models to code.

License
=======

It is released under the MIT license.


> Copyright (c) 2013 Hiroyuki Tanaka, 2017 Kira Inc.
>
> Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
