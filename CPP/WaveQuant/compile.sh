#!/bin/bash

g++ -std=c++11 -Wall -o wavequant src/main.cpp  src/wavequant.cpp -ljack -lfftw3 -lm
