#!/usr/bin/env sh
g++ -Wall -Werror -Wpedantic -std=c++14 -g src/main.cpp -o bin/Game $(sdl2-config --cflags --libs)
