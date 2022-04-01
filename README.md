# Zerr*

**Zerr**aeumlichung is a set of black box systems for live spatialization.
Each of the *Zerraeumlicher*-modules  receives a single input, distributing it to multiple output channels by means of different algorithms. Although the algorithms can be tuned, the only control over the spatialization is granted through the audio input.
**Zerr**aeumlichung does not work with the placing and movement of virtual sound sources, but rather with a spatial distribution[^1]. Properties of this distribution and the sounds' timbre, respectively their texture, are inherently connected.

[^1]: When space is used as a musical parameter, position and location are considered the equivalent to pitch and rhythm.

# Algorithms

## PeakShuffle

### FFT

### Peak Picking

### Spectral Splitting

### IFFT

## FluxCompensator

## ThreadWalk

# Dependencies

- libjack
- libfftw3



# Pixelize / Flux Compensator

- get the spectral flux for every 512 samples


aka *flux compensator*
