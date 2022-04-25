#include "weightshifter.h"

using std::cout;
using std::endl;

WeightShifter::WeightShifter()
{

  cout << "Reading ZERR config file" << endl;

  zerr_config    = YAML::LoadFile("/home/anwaldt/SOUND/Zerraeumlichung/config/zerr.yaml");
  YAML::Node cfg = zerr_config["zerr"];

  nOutputs       = cfg["n_outputs"].as<int>();
  cout << "Number of outputs: " << nOutputs << endl;;

  this->client = jack_client_open("WeightShifter", JackNullOption, &status, NULL);

  // connect the callback function
  jack_set_process_callback(this->client, this->callback_process, this);

  // allocate JACK ports and arrays
  input_port = new jack_port_t*[nInputs];
  for (int i=0; i<nInputs; i++)
  {
    std::string tmp_str = "input_" + std::to_string(i+1);
    input_port[i] = jack_port_register (client, tmp_str.c_str(),
    JACK_DEFAULT_AUDIO_TYPE,
    JackPortIsInput, 0);
  }

  output_port = new jack_port_t*[nOutputs];
  for (int i=0; i<nOutputs; i++)
  {
    std::string tmp_str = "output_" + std::to_string(i+1);
    output_port[i] = jack_port_register (client, tmp_str.c_str(),
    JACK_DEFAULT_AUDIO_TYPE,
    JackPortIsOutput, 0);
  }

  out = new jack_default_audio_sample_t*[nOutputs];
  in  = new jack_default_audio_sample_t*[nInputs];

  feature_interpolator = new LinearInterpolator();
  fft        = new FrequencyTransformer(L);
  features   = new FeatureMachine(L_fft);

  sprkmapper = new SpeakerMapper(nOutputs);
  sprkmapper->read_config("s");

  /// Activate and connect JACK stuff.

  jack_activate(this->client);

  // connect inputs
  jack_connect (client, "pure_data:output0", jack_port_name(input_port[0]));
  jack_connect (client, "PulseAudio JACK Sink:front-left", jack_port_name(input_port[0]));
  // connect outputs

  for(int chanCNT=0; chanCNT<nOutputs; chanCNT+=2)
  {
    // jack_connect (client, jack_port_name(output_port[chanCNT]), "system:playback_1");
    // jack_connect (client, jack_port_name(output_port[chanCNT]), "jaaa:in_1");
    //
    // jack_connect (client, jack_port_name(output_port[chanCNT+1]), "system:playback_2");
    // jack_connect (client, jack_port_name(output_port[chanCNT+1]), "jaaa:in_2");

  }
  // run forever
  sleep (-1);
}


int WeightShifter::process(jack_nframes_t nframes)
{

  /// Input/Output buffers and stuff:

  // get input buffers
  for ( int i=0 ; i<nInputs; i++)
  in[i]  = (jack_default_audio_sample_t *)
  jack_port_get_buffer(this->input_port[i], jack_get_buffer_size(client));

  // get output buffers
  for ( int i=0 ; i<nOutputs; i++)
  out[i] = (jack_default_audio_sample_t *)
  jack_port_get_buffer(this->output_port[i], jack_get_buffer_size(client));

  // shift FFT input by nframes
  for(int idx=0; idx<L-nframes; idx++)
  {
    fft->fft_input()[idx] = fft->fft_input()[idx+nframes];//* Zerr::get_hann_sample(idx,L);
  }

  // insert new samples
  for(int idx=0; idx<nframes; idx++)
  {
    // only take the first channel
    fft->fft_input()[L-nframes+idx] = in[0][idx];// * Zerr::get_hann_sample(L-nframes+idx,L);
  }

  // set output buffer "0.0"
  for(int chanCNT=0; chanCNT<nOutputs; chanCNT++)
  {
    for(int sampCNT=0; sampCNT<nframes; sampCNT++)
    out[chanCNT][sampCNT] = 0.0;
  }

  for(int sampCNT=0; sampCNT<nframes; sampCNT++)
  {

    if(hop_counter >= L_hop)
    {

      // fft and features
      fft->apply_window();
      fft->run_fft();
      fft->calculate_power_spectrum();

      // interpolations:
      float lastVal = centroid;
      centroid      = features->centroid(fft->power_spectrum());

      //  std::rand();//

      feature_interpolator->set_values(lastVal, centroid, L_hop);

      hop_counter = 0;

    }

    float val = feature_interpolator->get_value();

    Zerr::pair p = sprkmapper->pair_panner(val,0.0,1.0,2.0);

    feature_interpolator->next_step();

    // cout << val << "xxx" << p.s1 << " ::: " << p.s2 << endl;

    int s1 = sprkmapper->speaker_by_height(p.s1);
    int s2 = sprkmapper->speaker_by_height(p.s2);

    out[s1][sampCNT] = in[0][sampCNT]*p.g1;
    out[s2][sampCNT] = in[0][sampCNT]*p.g2;

    // // write all samples
    // for(int chanCNT=0; chanCNT<2; chanCNT++)
    // {
    //   out[chanCNT][sampCNT] = in[0][sampCNT];
    // }

    hop_counter += 1;

    // cout << val << " \n";
  }

   // cout << endl;

  return 0;
}

int WeightShifter::callback_process(jack_nframes_t x, void* object)
{
  return static_cast<WeightShifter*>(object)->process(x);
}
