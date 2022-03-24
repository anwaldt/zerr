#!/bin/bash

g++ -std=c++11 -Wall -o zerr src/main.cpp  src/zerr.cpp -ljack -lfftw3 -lm
