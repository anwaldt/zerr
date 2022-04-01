#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Mar 21 13:45:27 2022

@author: anwaldt
"""

sampCNT    = 0
ifft_index = 8
bufCNT     = 1
nframes    = 256
L          = 2048

for sampCNT in range(nframes):
    idx = (sampCNT + ((ifft_index-bufCNT) * nframes))%L
    print(idx)