#ifndef ZERR_H
#define ZERR_H

#include<stdlib.h>
#include<vector>
#include<algorithm>
#include<math.h>
#include<iostream>
#include<unistd.h>
#include <limits>

#include "yaml-cpp/yaml.h"

namespace Zerr {

#define PI 3.14159265

typedef struct
{
  int   s1, s2;
  float g1,g2;

} pair;

const double tiny = std::numeric_limits<double>::min();

inline float gaussian_lobe(int pos, double mu, double sigma, int L)
{
    double w     = exp( -(0.5) * pow( ((float) pos - mu) /sigma,2.0) );
    return w;
}



inline float get_hann_sample(int pos, int L)
{
    float val = 0.5 * (1.0 - cos( (2.0*PI* (float) pos) / (float)L) );
    return val;
}

///
/// \brief centroid
/// \param x a vector of doubles
/// \return
///
/// Normalized to fs/2



}

#endif //
