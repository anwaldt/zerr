#include "weightshifter.h"

using std::cout;
using std::endl;

WeightShifter::WeightShifter()
{

    // cout << "Starting Jack Client!" << endl;

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

    /// allocate and initialize FFT arrays

    n_buffers = L / jack_get_buffer_size(this->client);

    gain_mask = new float[nOutputs];
    for(int i=0; i<nOutputs; i++)
        gain_mask[i]=0.0;


    fft = new FrequencyTransformer(L);

    /// Activate and connect JACK stuff.

    jack_activate(this->client);

    // connect inputs
    jack_connect (client, "pure_data:output0", jack_port_name(input_port[0]));
    jack_connect (client, "PulseAudio JACK Sink:front-left", jack_port_name(input_port[0]));
    // connect outputs

    for(int chanCNT=0; chanCNT<nOutputs; chanCNT+=2)
    {
        jack_connect (client, jack_port_name(output_port[chanCNT]), "system:playback_1");
        jack_connect (client, jack_port_name(output_port[chanCNT]), "jaaa:in_1");

        jack_connect (client, jack_port_name(output_port[chanCNT+1]), "system:playback_2");
        jack_connect (client, jack_port_name(output_port[chanCNT+1]), "jaaa:in_2");

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
        fft->fft_input()[idx] = fft->fft_input()[idx+nframes];// * get_hann_sample(idx,L);
    }

    // insert new samples
    for(int idx=0; idx<nframes; idx++)
    {
        // only take the first channel
        fft->fft_input()[L-nframes+idx] = in[0][idx]; // * get_hann_sample(L-nframes+idx,L);
    }


    /// FFT / IFFT:

    float dist    = 0.0;

    if(hop_counter>=L_hop)
    {

        fft->run_fft();

        fft->calculate_power_spectrum();

        hop_counter=0;

        cout << Zerr::centroid(fft->power_spectrum()) << "\n";
    }


    /// Output to all channel buffers:

    // set output buffer "0.0"
    for(int chanCNT=0; chanCNT<nOutputs; chanCNT++)
    {
        for(int sampCNT=0; sampCNT<nframes; sampCNT++)
            out[chanCNT][sampCNT] = 0.0;
    }


    for(int sampCNT=0; sampCNT<nframes; sampCNT++)
    {
        // write all samples
        for(int chanCNT=0; chanCNT<nOutputs; chanCNT++)
        {
            out[chanCNT][sampCNT] += in[0][sampCNT] * gain_mask[chanCNT];
        }

    }


    hop_counter+=L_hop;

    return 0;
}

int WeightShifter::callback_process(jack_nframes_t x, void* object)
{
    return static_cast<WeightShifter*>(object)->process(x);
}
