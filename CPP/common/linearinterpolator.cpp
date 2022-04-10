#include "linearinterpolator.h"

LinearInterpolator::LinearInterpolator()
{
    y_start = 0;
    y_stop  = 0;

    L   = 1;
    pos = 0;
}

void LinearInterpolator::set_values(double start, double stop, int l)
{
    y_start = start;
    y_stop  = stop;

    L   = l;
    pos = 0;
}

double LinearInterpolator::get_value()
{
    float out = y_start + ((float) pos * ((y_stop - y_start) / (float) (L-1)));

    return out;
}

void LinearInterpolator::next_step()
{
    pos+=1;
}
