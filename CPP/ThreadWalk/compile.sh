#!/bin/bash

g++ -std=c++11 -Wall -o threadwalk src/main.cpp  src/threadwalk.cpp -ljack -lfftw3 -lm
