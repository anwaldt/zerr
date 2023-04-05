#include "speakermapper.h"

using std::cout;
using std::endl;

SpeakerMapper::SpeakerMapper(int n)
{

  n_speakers = n;

  speakers.resize(n_speakers,Speaker(0,0.0,0.0,0.0,0.0,0.0));

  // all sorted indices are ascending
  for (size_t i=0; i<n_speakers; i++)
  {
    xInds.push_back(i);
    yInds.push_back(i);
    zInds.push_back(i);
    aInds.push_back(i);
    eInds.push_back(i);
  }

}

void SpeakerMapper::read_config(std::string fileName)
{
  std::cout << "Reading speaker config:" << fileName << std::endl;

  // clear all existing data
  speakers.clear();
  xInds.clear();
  yInds.clear();
  zInds.clear();
  aInds.clear();
  eInds.clear();

  speaker_config = YAML::LoadFile(fileName);

  YAML::Node speakerNodes = speaker_config["speakers"];

  int idx = 0;

  for (auto it = speakerNodes.begin(); it != speakerNodes.end(); ++it)
  {

    YAML::Node key   = it->first;
    YAML::Node value = it->second;

    std::cout << "Speaker ID: " << key.as<std::string>() << "\n"; // Parent

    float x = value['x'].as<double>();
    float y = value['y'].as<double>();
    float z = value['z'].as<double>();
    float a = value['a'].as<double>();
    float e = value['a'].as<double>();

    std::cout << "\t x = " << x << "\n";
    std::cout << "\t y = " << y << "\n";
    std::cout << "\t z = " << z << "\n";
    std::cout << "\t a = " << a << "\n";
    std::cout << "\t e = " << e << "\n";

    Speaker s = Speaker(idx, x, y, z, a, e);

    speakers.push_back(s);

    idx++;
  }

  std::vector <Speaker> speakerSorter = speakers;

  // sort by X
  sort(speakerSorter.begin(), speakerSorter.end(), [](Speaker& lhs, Speaker& rhs) {
    return lhs.get_x() < rhs.get_x();
  });
  for(auto it = speakerSorter.begin(); it != speakerSorter.end(); it++)
  xInds.push_back(it->get_idx());

  // sort by y
  sort(speakerSorter.begin(), speakerSorter.end(), [](Speaker& lhs, Speaker& rhs) {
    return lhs.get_y() < rhs.get_y();
  });
  for(auto it = speakerSorter.begin(); it != speakerSorter.end(); it++){
    yInds.push_back(it->get_idx());}

    // sort by z
    sort(speakerSorter.begin(), speakerSorter.end(), [](Speaker& lhs, Speaker& rhs) {
      return lhs.get_z() < rhs.get_z();
    });
    for(auto it = speakerSorter.begin(); it != speakerSorter.end(); it++){
      zInds.push_back(it->get_idx());}

      // sort by directness
      // sort(speakerSorter.begin(), speakerSorter.end(), [](Speaker& lhs, Speaker& rhs) {
      //   return lhs.get_directness() < rhs.get_directness();
      // });
      // for(auto it = speakerSorter.begin(); it != speakerSorter.end(); it++){
      //   directnessInds.push_back(it->get_idx());}


        for(auto it = xInds.begin(); it != xInds.end(); it++)
        std::cout << *it << "\t";

        std::cout << std::endl;

        for(auto it = yInds.begin(); it != yInds.end(); it++)
        std::cout << *it << "\t";

        std::cout << std::endl;

        for(auto it = zInds.begin(); it != zInds.end(); it++)
        std::cout << *it << "\t";

        std::cout << std::endl;

      }

      int SpeakerMapper::random_speaker()
      {

      }

      Zerr::pair SpeakerMapper::pair_panner(float in, float min, float max, float exponent)
      {

        float d = std::abs(min - max);

        float scaled =  std::pow( ((1.0/d) * (in - min)) * ((float) n_speakers - 1.0) , exponent);;

        int lower = std::max(0,std::min(n_speakers-1, (int)floor(scaled)));
        int upper = std::max(0,std::min(n_speakers-1, (int)ceil(scaled)));

        // keep frac within 0.0 - 1.0 to avoid clipping
        double frac = std::max((float)0.0,  std::min((float) 1.0, scaled-lower));


        //  cout << "Scaled:" << scaled<< endl;
        //  cout << "Frac:" << frac << endl;

        //cout << "OUT 2:" << p.s2 << " ::: " << p.g2 << endl;

        Zerr::pair p = {};

        p.s1 = lower;
        p.s2 = upper;
        p.g2 = frac;
        p.g1 = 1.0-frac;

        return p;

      }

      int SpeakerMapper::speaker_by_x(int spkrIdx)
      {
        return xInds[spkrIdx];
      }
      int SpeakerMapper::speaker_by_y(int spkrIdx)
      {
        return yInds[spkrIdx];
      }
      int SpeakerMapper::speaker_by_z(int spkrIdx)
      {
        return zInds[spkrIdx];
      }
