# CHIP-8 Emulator

A CHIP-8 emulator written in C++ using SDL2. This project was developed on Windows using Visual Studio Code. This emulator was created as a learning project. As such, there may be bugs or incomplete features. Use it at your own discretion and feel free to report any issues or contribute improvements.

## Table of Contents
- Introduction
- Requirements
- Installation
- Usage
- License

## Introduction
CHIP-8 is an interpreted programming language, developed in the 1970s, used to create simple games. This emulator allows you to run CHIP-8 programs and games on your computer.


## Requirements
- [MinGW-w64 GCC compiler](https://www.msys2.org/)
- [git](https://git-scm.com/downloads)
- [cmake](https://cmake.org/download/)
- Was tested in windows, instructions are for windows, but should work in other operating systems. 

## Installation
- `git clone https://github.com/r17v1/chip8.git`
- Open the folder containing the downloaded repository (Should contain `src` and `ROMS` folders)
- Open this folder in terminal
- `mkdir build`
- `cd build`
- `cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..` (note: just `cmake ..` might work in linux, or even windows if your defult compilers are `gcc` and `g++`. The build will fail for `msvc`)
- `cmake --build .`

## Usage 
- You need to download `sdl2.dll`. Download a [build here](https://github.com/libsdl-org/SDL/releases/tag/release-2.30.8) 
- put the dll inside the build folder.
- opening the .exe file will give you an error if the dll is missing. 
- inside build folder run `./chip8.exe file_path` eg: `./chip8.exe ../ROMS/BRIX.ch8`
- Controls: 1 2 3 4 q w e r a s d f z x c v

## License

MIT License

Copyright (c) [2024] [Rizvi Hasan]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
