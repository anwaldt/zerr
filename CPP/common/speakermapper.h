#ifndef SPEAKERMAPPER_H
#define SPEAKERMAPPER_H

#include<stdlib.h>
#include<iostream>
#include"zerr.h"
#include "speaker.h"
#include<cmath>

class SpeakerMapper
{

public:

    SpeakerMapper(int n);

    void read_config(std::string fileName);

    int random_speaker();

    int speaker_by_height(int spkrIdx);

    Zerr::pair pair_panner(float in, float min, float max, float exponent);

private:

    std::vector <Speaker> speakers;
    std::vector <Speaker> outputs;

    int n_speakers = 0;

    YAML::Node speaker_config;

    std::vector <int> xInds;
    std::vector <int> yInds;
    std::vector <int> zInds;
    std::vector <int> directnessInds;

};

#endif // SPEAKERMAPPER_H
