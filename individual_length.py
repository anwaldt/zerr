 
 
import matplotlib.pyplot as plt

with open('tester.txt') as f:
    lines = f.read().splitlines()


line    = lines[0]
a       = []    
strings = line.split(' ')

for s in strings[0:-2]:
    a.append(float(s))
        
plt.plot(a)



