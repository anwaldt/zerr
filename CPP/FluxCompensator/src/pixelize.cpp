#include "pixelize.h"

using std::cout;
using std::endl;

Pixelize::Pixelize()
{

    // cout << "Starting Jack Client!" << endl;

    this->client = jack_client_open("Pixelize", JackNullOption, &status, NULL);

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


    fft_in = new double[L];
    for(int i=0; i<L; i++)
        fft_in[i]=0;

    fft_out = new fftw_complex[L_fft];

    power_spectrum.resize(L_fft);
    last_power_spectrum.resize(L_fft);

    p_fft  =  fftw_plan_dft_r2c_1d(L, fft_in,  fft_out,   FFTW_ESTIMATE);

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



int Pixelize::process(jack_nframes_t nframes)
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
        fft_in[idx] = fft_in[idx+nframes];// * get_hann_sample(idx,L);
    }

    // insert new samples
    for(int idx=0; idx<nframes; idx++)
    {
        // only take the first channel
        fft_in[L-nframes+idx] = in[0][idx]; // * get_hann_sample(L-nframes+idx,L);
    }


    /// FFT / IFFT:

    float dist    = 0.0;

    if(hop_counter>=L_hop)
    {

        fftw_execute(p_fft);

        last_spec_max = spec_max;

        spec_max      = 0.0;


        // get new power spectrum
        for(int tmpCNT = 0;tmpCNT<L_fft; tmpCNT++)
        {

            // remember last power spectrum value
            last_power_spectrum[tmpCNT] = power_spectrum[tmpCNT];

            power_spectrum[tmpCNT] = (1.0 / (2.0*(float) L)) * (fft_out[tmpCNT][0] *fft_out[tmpCNT][0]  + fft_out[tmpCNT][1]*fft_out[tmpCNT][1]);

            if(power_spectrum[tmpCNT]>spec_max)
                spec_max = power_spectrum[tmpCNT];

            if(last_spec_max>0)
                dist += pow((power_spectrum[tmpCNT] - last_power_spectrum[tmpCNT]) / last_spec_max ,2);

        }

        if(dist!=0.0)
            dist = sqrt(dist)*flux_normalizer;

        //        cout << dist << endl;

        hop_counter=0;

    }


    float dispersion = std::max((float) 0.0, std::min((float) 1.0, (float) pow(1.0 - dist, 1.0)));
    // float dispersion = std::max((float) 0.0, std::min((float)1.0, dist));

    cout << dispersion << endl;

    /// Output to all channel buffers:

    // set output buffer "0.0"
    for(int chanCNT=0; chanCNT<nOutputs; chanCNT++)
    {
        for(int sampCNT=0; sampCNT<nframes; sampCNT++)
            out[chanCNT][sampCNT] = 0.0;
    }



    for(int sampCNT=0; sampCNT<nframes; sampCNT++)
    {


        for(int i=0; i<nOutputs; i++)
            gain_mask[i]= (1.0 - dispersion);

        int center = rand()%nOutputs;

        gain_mask[center] = dispersion;

        // write all samples
        for(int chanCNT=0; chanCNT<nOutputs; chanCNT++)
        {


            out[chanCNT][sampCNT] += in[0][sampCNT] * gain_mask[chanCNT];

        }
    }

    ifft_index++;
    if(ifft_index>=n_buffers)
        ifft_index=0;

    hop_counter+=L_hop;


    return 0;
}

int Pixelize::callback_process(jack_nframes_t x, void* object)
{
    return static_cast<Pixelize*>(object)->process(x);
}
