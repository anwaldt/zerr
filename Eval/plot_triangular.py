#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Mar 23 10:43:04 2022

@author: anwaldt
"""

import csv
from numpy import loadtxt, genfromtxt, linspace
import matplotlib.pyplot as plt

lines = genfromtxt('tester.txt', delimiter=' ')[:,:-1]

N         = len(lines)

n_overlap = 16

s         = 0

for i in range(128):
    
    #i=i*n_overlap
    line = lines[i]  
    L    = len(line)

    n = linspace(s,s+L-1,L)        
    plt.plot(n,line)
    s=s+L
    #s=s+256
    