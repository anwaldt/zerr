#ifndef SHIFTER_H
#define SHIFTER_H

#include<stdlib.h>
#include<vector>
#include<algorithm>
#include<math.h>
#include<iostream>
#include<unistd.h>

#define PI 3.14159265


class Shifter
{
public:
    Shifter(int n_outputs);

    void shift(int fs);
    int speaker_a();

    int speaker_b();

    float get_interpolator();

    float get_route();

private:

    float f     = 1;
    float phi   = 0.0;

    float route = 0.0;

    float interp = 0.0;



    int n_outputs  = 2;

    int lower = 0;
    int upper = 1;

    int next_support_point;

    int   speakers[2] = {0,1};
    float gains[2]    = {0.0,0.0};

    float derivative = 0.0;
    float dir      = 1;

    float last_dir = 1;



    void next_speaker_rand();

    void get_next_support_point(float this_phi, int fs);

};

#endif // SHIFTER_H
