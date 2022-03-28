#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Mar 23 22:56:13 2022

@author: anwaldt
"""


from numpy import loadtxt, genfromtxt, linspace, exp
import matplotlib.pyplot as plt

L     = 512

n     = linspace(0, L-1,L)

sigma = 10

mu    = 100

w     = exp( -(0.5) * pow( ((n/L)-(mu/L)) / (sigma/L),2) )

plt.plot(w)