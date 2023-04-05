#ifndef SPEAKER_H
#define SPEAKER_H

#include"zerr.h"

class Speaker
{
public:

    Speaker(int idx, float x, float y, float z, float a, float d);

    // all the getters
    float get_x(){return x;};
    float get_y(){return y;};
    float get_z(){return z;};
    float get_a(){return a;};
    float get_e(){return e;};

    float get_idx(){return idx;};

private:

    int idx = 0;
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;

    float a  = 0.0;
    float e  = 0.0;

};

#endif // SPEAKER_H
