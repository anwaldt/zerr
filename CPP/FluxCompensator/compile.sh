#!/bin/bash

g++ -std=c++11 -Wall -o pixelize src/main.cpp  src/pixelize.cpp -ljack -lfftw3 -lm
