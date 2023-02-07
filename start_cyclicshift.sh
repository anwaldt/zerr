#!/bin/bash


tmux new -s CYCLIC \; \
  send-keys 'cyclicshift  -z config/zerr.yaml -s config/line_8.yaml'  C-m \; \
  split-window -v \; \
  send-keys 'sclang SC/speaker_mapper.scd'  C-m \; \
  split-window -v \; \
  send-keys 'pd Eval/tester.pd'  C-m \; \

 


