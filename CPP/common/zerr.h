#ifndef ZERR_H
#define ZERR_H

#include <limits>

namespace Zerr {

#define PI 3.14159265

inline float gaussian_lobe(int pos, double mu, double sigma, int L)
{
    double w     = exp( -(0.5) * pow( ((float) pos - mu) /sigma,2.0) );
    return w;
}

inline float centroid(std::vector <double> x)
{

    double k     = 0;
    double num   = 0.0000;
    double denum = 0.00001;

    for(auto it = std::next(begin(x)); it != std::end(x); ++it) {

        num   += k* *it;
        denum += *it;

        k+=1;
    }

    float c = (num / denum) / x.size();

    return c;
}

}

#endif //
