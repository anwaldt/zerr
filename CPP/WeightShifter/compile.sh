#!/bin/bash

g++ -std=c++11 -Wall -o  weightshifter -I../common/ src/main.cpp src/weightshifter.cpp ../common/frequencytransformer.cpp ../common/speakermapper.cpp ../common/speaker.cpp ../common/linearinterpolator.cpp -ljack -lfftw3 -lm
