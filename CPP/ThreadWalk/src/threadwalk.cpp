#include "threadwalk.h"

using std::cout;
using std::endl;

ThreadWalk::ThreadWalk()
{

    // cout << "Starting Jack Client!" << endl;
    this->client = jack_client_open("ThreadWalk", JackNullOption, &status, NULL);

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


    /// Activate and connect JACK stuff.

    jack_activate(this->client);

    // connect inputs
    // jack_connect (client, "pure_data:output0", jack_port_name(input_port[0]));
    // jack_connect (client, "PulseAudio JACK Sink:front-left", jack_port_name(input_port[0]));
    // // connect outputs
    //
    // for(int chanCNT=0; chanCNT<nOutputs; chanCNT+=2)
    // {
    //     jack_connect (client, jack_port_name(output_port[chanCNT]), "system:playback_1");
    //     jack_connect (client, jack_port_name(output_port[chanCNT]), "jaaa:in_1");
    //
    //     jack_connect (client, jack_port_name(output_port[chanCNT+1]), "system:playback_2");
    //     jack_connect (client, jack_port_name(output_port[chanCNT+1]), "jaaa:in_2");
    //
    // }
    // run forever
    sleep (-1);
}



int ThreadWalk::process(jack_nframes_t nframes)
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


    /// Output to all channel buffers:

    // set output buffer "0.0"
    for(int chanCNT=0; chanCNT<nOutputs; chanCNT++)
    {
        for(int sampCNT=0; sampCNT<nframes; sampCNT++)
            out[chanCNT][sampCNT] = 0.0;
    }


    for(int sampCNT=0; sampCNT<nframes; sampCNT++)
    {

        // shift and fill the sample buffer
        sample_buffer[1] = sample_buffer[0];
        sample_buffer[0] = in[0][sampCNT];

        int sign1 =   (sample_buffer[0] > 0) - (sample_buffer[0] < 0);
        int sign2 =   (sample_buffer[1] > 0) - (sample_buffer[1] < 0);

        if(sign1 != sign2)
        {

            out_index = rand()%nOutputs;

            // cout << sample_buffer[0] << " - " << sample_buffer[1] << endl;
        }

        out[out_index][sampCNT] = in[0][sampCNT];
    }

    return 0;
}

int ThreadWalk::callback_process(jack_nframes_t x, void* object)
{
    return static_cast<ThreadWalk*>(object)->process(x);
}
