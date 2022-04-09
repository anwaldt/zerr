#ifndef ZERR_H
#define ZERR_H

#include<stdlib.h>
#include<vector>
#include<algorithm>
#include<math.h>
#include<iostream>
#include<unistd.h>

namespace Zerr {

#define PI 3.14159265

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

inline float centroid(std::vector <double> x)
{

    double k     = 0;
    double num   = 0.0000;
    double denum = 0.00001; // use a tiny offset to avoid NaNs

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

}

#endif //
