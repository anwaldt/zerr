#!/bin/bash

CPP/build/WeightShifter/weightshifter  -z config/zerr.yaml -s config/line_8.yaml &

meterbridge -r 0dB -c 8 -n meter -t dpm `seq 7` &

pd Eval/tester.pd &
