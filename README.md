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


## Requirements (URLs attached for windows)
- [MinGW-w64 GCC compiler](https://www.msys2.org/)
- [git](https://git-scm.com/downloads)
- [cmake](https://cmake.org/download/)
- Was tested in windows, mac os and ubuntu. 

## Installation
- `git clone https://github.com/r17v1/chip8.git`
- Open the `chip8` folder (Should contain `src` and `ROMS` folders) in terminal
- `mkdir build`
- `cd build`
- `cmake ..` *(Note: for windows you might have to run `cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..` if your defult compilers are not `gcc` and `g++` since `msvc` is not supported)*
- `cmake --build .`

## Usage 
- For windows you need to download `sdl2.dll`. Download a [build here](https://github.com/libsdl-org/SDL/releases/tag/release-2.30.8) put the dll inside the build folder.
- inside build folder run `./chip8`. Optional args `--file_path=path to rom`, `--fps=fps` and `--clock_speed=clock speed` can be added. Eg: `./chip8 --file_path=../ROMS/BRIX.ch8 --fps=60 --clock_speed=700` 
- `./chip8 --help` can be used to see instructions. 
- Controls: 1 2 3 4 q w e r a s d f z x c v