#include "featuremachine.h"

FeatureMachine::FeatureMachine(int fft_size)
{

  last_power_spectrum.resize(fft_size,0.0);

}

float FeatureMachine::centroid(std::vector <double> x)
{

    double k       = 0;
    double denum   = 0.0000;
    double num     = 0.00001; // use a tiny offset to avoid NaNs

    // from k=1 to K:
    for(auto it = std::next(begin(x)); it != std::end(x); ++it)
    {
        num   += k* *it;
        denum += *it;

        k+=1;
    }

    float c = (num / denum) / (float) x.size();

    return c;
}

float FeatureMachine::flatness(std::vector <double> x)
{

  double denum  = 0.0000;
  double num    = 0.0000; // use a tiny offset to avoid NaNs

  // from troedeldoedel=1 to TROEDELDOEDEL:
  for(auto it = std::begin(x); it != std::end(x); ++it)
  {
      denum += std::log(*it + Zerr::tiny);
      num   += *it + Zerr::tiny;
  }

  float N = (float) x.size();

  float flatness = std::exp(denum / N) / (num/N);


  return flatness;

}

float FeatureMachine::spectral_flux(std::vector <double> power_spectrum)
{

  float spec_max      = 0;
  float dist          = 0;

  // get new power spectrum
  for(int tmpCNT = 0; tmpCNT<n_fft; tmpCNT++)
  {

      if(power_spectrum[tmpCNT]>spec_max)
          spec_max = power_spectrum[tmpCNT];

      if(last_spec_max>0)
          dist += pow((power_spectrum[tmpCNT] - last_power_spectrum[tmpCNT]) / last_spec_max ,2);

      // remember last power spectrum value
      last_power_spectrum[tmpCNT] = power_spectrum[tmpCNT];

  }

  last_spec_max = spec_max;

  return dist;

}


int FeatureMachine::zero_crossing(double s)
{

  int sign1 =   (s > 0) - (s < 0);
  int sign2 =   (last_sample > 0) - (last_sample < 0);

  last_sample = s;

  return 0.5 * (sign1 - sign2);

}
