# Zerr*

**Zerr***aeumlichung is a set of black box systems for signal-driven live spatialization.
Each of the **Zerr***aeumlicher-modules  receives a single input, distributing it to multiple output channels by means of different algorithms. Although the algorithms can be tuned, the only control over the spatialization is granted through the audio input.
**Zerr***aeumlichung does not work with the placing and movement of virtual sound sources, but rather with a spatial distribution[^1]. Properties of this distribution and the sounds' timbre, respectively their texture, are inherently connected.


## Dependency of Reproduction Setup

The restults of the signal-driven spatialization strongly depend on the reproduction setup, respectively the loudspeaker configuration.


## Modulation Sources & Operations

### Sources

Time domain

Frequency Domain

### Operations



## Truly Spatiomorph Algorithms

Truly spatiomorph algorithms change only the spatial distribution of the sound.
When superimposing all loudspeaker signals $y_n$ , the original signal $x$ is reproduced.

[^1]: When space is used as a musical parameter, position and location are considered the equivalent to pitch and rhythm.

## Related works:

*Spectromorphology and Spatiomorphology of Sound Shapes:
Audio-Rate AEP and DBAP Panning of Spectra* (Stuart James, 2015)


# Algorithms

## WaveQuant

- Source: sample-wise time domain gain
- Manipulation: sample-wise interpolation between two speakers

WaveQuant is based on a quantization grid with $N_outputs$ quantization steps, as shown here for 7 outputs:

![WaveQuant steps for N=7 outputs.](Graphics/wavequant.svg)

For each incoming sample, the lower and upper quantization points are used as direct loudspeaker indices and the gain for both is calculated as follows:


## ThreadWalk

- Source: time domain zero crossings
- Modulation: Shift to random speaker

ThreadWalk

## CycleShift

sinusiodal movement through speakers

- fixed order
- random order


## FluxCompensator

- Source: Spectral Flux
- Modulation: random assignment of bins to speakers, using the flux for the strength

## PeakShuffle

### FFT

### Peak Picking

### Spectral Splitting

### IFFT



## Pixelize / Flux Compensator

- get the spectral flux for every 512 samples


aka *flux compensator*



# Install

Build and install works with meson + ninja. From the rot directory of the repository do:

1:
  meson build

2:
  ninja -C build/

3:
  sudo meson install -C build/

## Dependencies

- pip3 install meson
- ninja
- libjack2-dev
- libfftw3-dev
- libyaml-cpp-dev
