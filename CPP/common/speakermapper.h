#ifndef SPEAKERMAPPER_H
#define SPEAKERMAPPER_H

#include"zerr.h"
#include "speaker.h"

class SpeakerMapper
{

public:

    SpeakerMapper(int n);

    void read_config(std::string fileName);

    int random_speaker();

    int speaker_by_height(int spkrIdx);

    Zerr::pair pair_panner(float in, float min, float max);

private:

    std::vector <Speaker> speakers;
    int n_speakers = 0;

    std::vector <std::pair <int, double>> heights;
    std::vector <std::pair <int, double>> front;
    std::vector <std::pair <int, double>> left;

};

#endif // SPEAKERMAPPER_H
