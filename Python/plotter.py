

import matplotlib.pyplot as plt
import numpy as np
from tikzplotlib import save as tikz_save
from scipy import signal, optimize


t = np.arange(0.0, 2, 0.01)

#######################################################################################################


plt.figure(0)

def fun(t):
    
    return 0.5* (( np.sin(2* np.pi * t )) + ( .95 * np.sin(3.2* 2* np.pi * t )))
    

r = optimize.fsolve(fun,[1,1])


s = fun(t)

plt.plot(t,t*0, color = (0.0,0.0,0.0) )

plt.plot(t, s, lw=2.1  , color = (0.3,0.3,0.3) )

plt.scatter([0.476, 0.952, 1.14, 1.18, 1.43, 1.59, 1.66, 1.905],[0,0,0,0,0,0,0,0],linestyle="None", marker='o', s=200, color = (0.1,0.1,0.1))


plt.xlabel("t/s")
plt.ylabel("x(t)")
#plt.title("Sinusoidal")

ax = plt.gca()

ax.set_facecolor((1,1,1))

#plt.grid(color='k', linestyle='-', linewidth=0.2)

plt.ylim(-1.0,1.0) 
plt.xlim(0,2) 

tikz_save('zero-crossings.tex')#, figureheight='0.5\\textwidth', figurewidth='\\textwidth')



#######################################################################################################


plt.figure()

t = np.arange(0.0, 0.2, 0.01)
s = fun(t)


for i in range(8):

    plt.plot(t,t*0+ 2*((i)/(8-1))-1, lw=4,color = (0.0,0.0,0.0) )

plt.plot(t, s, lw=2.1  , color = (0.3,0.3,0.3) )

plt.scatter([0.0117, 0.036],[0.144, 0.426],linestyle="None", marker='o', s=1, color = (0.1,0.1,0.1))

#plt.scatter([0.0249],[0.298],linestyle="None", marker='X', s=1, color = (0.1,0.1,0.1))
plt.ylim(-1.0,1.0) 


tikz_save('quantize.tex')#, figureheight='0.5\\textwidth', figurewidth='\\textwidth')
