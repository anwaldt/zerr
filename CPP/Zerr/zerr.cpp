#include "zerr.h"


using std::cout;
using std::endl;


Zerr::Zerr()
{

    //cout << "Starting Jack Client!" << endl;

    this->client = jack_client_open("Zerr", JackNullOption, &status, NULL);

    // connect the callback function
    jack_set_process_callback(this->client, this->callback_process, this);


    // allocate array
    input_port = new jack_port_t*[nChannels];
    for (int i=0; i<nChannels; i++)
    {
        std::string tmp_str = "input_" + std::to_string(i+1);
        input_port[i] = jack_port_register (client, tmp_str.c_str(),
                                            JACK_DEFAULT_AUDIO_TYPE,
                                            JackPortIsInput, 0);
    }

    output_port = new jack_port_t*[nChannels];
    for (int i=0; i<nChannels; i++)
    {
        std::string tmp_str = "output_" + std::to_string(i+1);
        output_port[i] = jack_port_register (client, tmp_str.c_str(),
                                             JACK_DEFAULT_AUDIO_TYPE,
                                             JackPortIsOutput, 0);
    }

    out = new jack_default_audio_sample_t*;
    in  = new jack_default_audio_sample_t*;


    // allocate and initialize FFT arrays
    n_buffers = L / jack_get_buffer_size(this->client);

    //    cout << L << endl;
    //    cout << jack_get_buffer_size(this->client) << endl;
    //    cout << n_buffers << endl;

    fft_buff = new fftw_real[L];
    for(int i=0; i<=L; i++)
        fft_buff[i]=0;

    fft_in = new fftw_real[L];
    for(int i=0; i<=L; i++)
        fft_in[i]=0;

    fft_out = new fftw_real[L];
    for(int i=0; i<=L; i++)
        fft_out[i]=0;


    ifft_out = new fftw_real*[n_buffers];
    for(int i=0; i<=n_buffers; i++)
        ifft_out[i] = new fftw_real[L];

    power_spectrum = new fftw_real[L/2+1];
    for(int i=0; i<=(L/2); i++)
        power_spectrum[i] = 0;



    p_fft  = rfftw_create_plan(L, FFTW_REAL_TO_COMPLEX, FFTW_ESTIMATE);
    p_ifft = rfftw_create_plan(L, FFTW_COMPLEX_TO_REAL, FFTW_ESTIMATE);




    jack_activate(this->client);

    // connect inputs
    jack_connect (client, "pure_data:output0", jack_port_name(input_port[0]));
    jack_connect (client, "pure_data:output1", jack_port_name(input_port[1]));
    // connect outputs
    jack_connect (client, jack_port_name(output_port[0]), "system:playback_1");
    jack_connect (client, jack_port_name(output_port[1]), "system:playback_2");

    // run forever
    sleep (-1);
}




int Zerr::process(jack_nframes_t nframes)
{


    // get input buffers
    for ( int i=0 ; i<nChannels; i++)
    {
        out[i] = (jack_default_audio_sample_t *)
                jack_port_get_buffer(this->output_port[i], jack_get_buffer_size(client));
        in[i]  = (jack_default_audio_sample_t *)
                jack_port_get_buffer(this->input_port[i], jack_get_buffer_size(client));

    }

    // shift FFT input by nframes
    for(int idx=L-1; idx>=L-nframes-1; idx--)
        fft_in[idx] =fft_in[idx-nframes];//*get_hann_sample(idx,L);

    // insert new samples
    for(int idx=0; idx<=nframes; idx++)
        // only take the first channels
        fft_in[idx] = in[0][idx];//*get_hann_sample(idx,L);



    rfftw_one(p_fft, fft_in, fft_out);

    // magniture of DC component:
    power_spectrum[0] = fft_out[0]*fft_out[0];

    // get power by real*imag (k < N/2 rounded up)
    for (int k = 1; k < (L+1)/2; ++k)
    {
        power_spectrum[k] = fft_out[k]*fft_out[k] + fft_out[L-k]*fft_out[L-k];
    }

    if (L % 2 == 0) /* N is even */
        power_spectrum[L/2] = fft_out[L/2]*fft_out[L/2];  /* Nyquist freq. */

    // IFFT
    rfftw_one(p_ifft, fft_out, ifft_out[ifft_index]);

    // delete-loop
    for(int chanCNT=0; chanCNT<nChannels; chanCNT++)
    {
        for(int sampCNT=0; sampCNT<nframes; sampCNT++)
            out[chanCNT][sampCNT] = 0.0;
    }

    // write all input samples to output
    for(int chanCNT=0; chanCNT<nChannels; chanCNT++)
    {

        // the overlap add
        for(int bufCNT=0; bufCNT<n_buffers; bufCNT++)
        {

            uint buf_ind=0;
            float outsamp = 0;
            for(int sampCNT=0; sampCNT<nframes; sampCNT++)
            {

                outsamp = fft_in[buf_ind];

                // the INDIVIDUAL ola index for each buffer
                // buf_ind = (sampCNT + ((ifft_index-bufCNT) * nframes))%L;

                outsamp =  get_triangular_sample(buf_ind,L);
                //outsamp = ifft_out[bufCNT][buf_ind] * get_triangular_sample(buf_ind,L);

                out[chanCNT][sampCNT] += outsamp; // /100.0;

                cout << outsamp << " ";

            }

            // cout << buf_ind << " ";
            cout << endl;
        }
    }

    ifft_index++;
    if(ifft_index>=n_buffers)
        ifft_index=0;

    return 0;
}



int Zerr::callback_process(jack_nframes_t x, void* object)
{
    return static_cast<Zerr*>(object)->process(x);
}

float Zerr::get_hann_sample(float pos, int L)
{
    float val = (1.0/(float) L)* (0.5 + 0.5*cos((2.0*PI*pos)/(float)L));
    return val;
}


float Zerr::get_triangular_sample(float pos, int L)
{
    float val = 0;

    if( ((float) pos) < ((float) L / 2.0))
        val  = (1.0 / ( L/2.0)) *   pos;
    else
        val =  1.0 - ((1.0 / ( L/2.0))  *   (pos-(L/2)));

    return val;
}

