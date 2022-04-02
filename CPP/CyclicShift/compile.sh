#!/bin/bash

g++ -std=c++11 -Wall -o cyclicshift src/main.cpp  src/cyclicshift.cpp src/shifter.cpp -ljack -lfftw3 -lm
