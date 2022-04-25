#ifndef FEATUREMACHINE_H
#define FEATUREMACHINE_H

#include "zerr.h"

class FeatureMachine
{
public:

    FeatureMachine(int fft_size);

    float centroid(std::vector <double> x);
    float flatness(std::vector <double> x);

    float spectral_flux(std::vector <double> power_spectrum);

    int zero_crossing(double s);

private:

  int n_fft;

  std::vector <float> last_power_spectrum;
  float last_spec_max = 0;

  // buffers a sample for zero crossing
  double last_sample = 0.0;

};

#endif // FEATUREMACHINE_H
