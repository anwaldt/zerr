#!/bin/bash

weightshifter  -z config/zerr_16.yaml -s config/line_16.yaml &

meterbridge -r 0dB -c 16 -n meter -t dpm `seq 15` &

pd Eval/tester.pd &
