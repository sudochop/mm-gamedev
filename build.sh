#!/usr/bin/env sh
gcc -c -std=c99 -o bin/ini.o src/vendor/inih/ini.c
g++ -c -o bin/INIReader.o src/vendor/inih/cpp/INIReader.cpp
g++ -Wall -Werror -Wpedantic -std=c++14 -g src/main.cpp -o bin/Game bin/INIReader.o bin/ini.o -lSDL2 -lSDL2_ttf -lSDL2_image
