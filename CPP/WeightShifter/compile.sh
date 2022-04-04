#!/bin/bash

g++ -std=c++11 -Wall -o  weightshifter -I../common/ src/main.cpp src/weightshifter.cpp ../common/frequencytransformer.cpp -ljack -lfftw3 -lm
