#include "speakermapper.h"

SpeakerMapper::SpeakerMapper(int n_speakers)
{

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

int SpeakerMapper::speaker_by_height(int sprIdx)
{

//    std::sort(v.begin(), v.end(), [](auto &left, auto &right) {
//        return left.second < right.second;
//    });

}
