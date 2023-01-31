#!/bin/bash

CPP/build/WeightShifter/weightshifter  -z config/zerr.yaml &
meterbridge -r 0dB -c 8 -n meter -t dpm `seq 7`
