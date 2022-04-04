import csv
from numpy import loadtxt, genfromtxt, linspace
import matplotlib.pyplot as plt


lines = genfromtxt('../tester.txt', delimiter=' ')[:,:-1]


N    = len(lines)




s = 0

for i in range(16):
    
    i=i*16
    line = lines[i]  
    L    = len(line)

    n = linspace(s,s+L-1,L)        
    plt.plot(n,line)
    s=s+L
    