#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Apr  4 22:22:21 2022

@author: anwaldt
"""

import csv
from numpy import loadtxt, genfromtxt, linspace
import matplotlib.pyplot as plt


lines = genfromtxt('../tester.txt', delimiter=' ')[:,:-1]


N    = len(lines)




s = 0

line = lines[i]  
L    = len(line)

n = linspace(s,s+L-1,L)        
plt.plot(n,line)
    