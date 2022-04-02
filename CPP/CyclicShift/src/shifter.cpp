#include "shifter.h"

Shifter::Shifter(int n_out)
{
    n_outputs = n_out;
}

void Shifter::get_next_support_point(float this_phi, int fs)
{

    float tmp_route  = (0.5+0.5*sin(this_phi)) * (n_outputs-1);


}

void Shifter::shift(int fs)
{

    // count argument and get routing value
    phi   = phi + (2.0*PI*f *(1.0/fs)) , 2.0*PI;

    // route   = 0.01 + (1.0 - 0.02)*(0.5+0.5*sin(phi)) * (n_out-1);

    route = (0.5+0.5*sin(phi)) * (n_outputs-1);

    derivative = cos(phi);

    dir        = (derivative > 0) - (derivative < 0);

    // check if we are between two new integers
    if(floor(route)!=lower && ceil(route)!= upper )
    {

        // draw new random speaker
        next_speaker_rand();



        lower = floor(route);
        upper = ceil(route);

//        std::cout << "Next speaker pair: "<< dir <<  " (direction) : ";
//        std::cout << lower << " ---" << upper << "  <> ";
//        std::cout << speakers[0] << " -> " << speakers[1] << std::endl;

    }


    // interpolation depends on slope
    if(dir>0)
        interp = route - (float) lower;
    else
        interp = 1.0 - (route - (float) lower);

    // flip speakers if sign of derivative changes
    if(last_dir!=dir)
    {
//        std::cout << "Speaker flip!" << std::endl;

        int s = speakers[0];
        speakers[0] = speakers[1];
        speakers[1] = s;
    }

    last_dir = dir;
}

float Shifter::get_interpolator()
{
    return interp;
}

void Shifter::next_speaker_rand()
{

    speakers[0] = speakers[1];
    while(speakers[0] == speakers[1] )
        speakers[1] = rand()%n_outputs;

}

int Shifter::speaker_a()
{
    return speakers[0];
}

int Shifter::speaker_b()
{
    return speakers[1];
}
float Shifter::get_route()
{
    return route;
}
