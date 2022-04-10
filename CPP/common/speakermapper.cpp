#include "speakermapper.h"

SpeakerMapper::SpeakerMapper(int n)
{

    n_speakers = n;

    speakers.resize(n_speakers);

    // speakers.resize(n_speakers, std::make_pair(0,0.0));

    for (size_t i=0; i<n_speakers; i++)
    {
         heights.push_back(std::make_pair(0,0.0));
    }

}

void SpeakerMapper::read_config(std::string fileName)
{



}

int SpeakerMapper::random_speaker()
{

}

Zerr::pair SpeakerMapper::pair_panner(float in, float min, float max)
{

  float d = abs(min - max);

  float scaled =  (d*0.5 + d*0.5*in) * ((float) n_speakers - 1.0);


  int lower = std::max(0,std::min(n_speakers-1, (int)floor(scaled)));
  int upper = std::max(0,std::min(n_speakers-1, (int)ceil(scaled)));

  int frac = scaled-lower;

  Zerr::pair p = {.s1 = lower, .s2=upper, .g1=frac, .g2=1.0-frac};

}

int SpeakerMapper::speaker_by_height(int sprIdx)
{

//    std::sort(v.begin(), v.end(), [](auto &left, auto &right) {
//        return left.second < right.second;
//    });

}
