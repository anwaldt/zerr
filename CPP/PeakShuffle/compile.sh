#!/bin/bash

g++ -std=c++11 -Wall -o peakshuffle src/main.cpp  src/peakshuffle.cpp -ljack -lfftw3 -lm
