
#include "zerr.h"

using std::cout;
using std::endl;

Zerr::Zerr()
{

    //    cout << "Starting Jack Client!" << endl;

    this->client = jack_client_open("Zerr", JackNullOption, &status, NULL);

    // connect the callback function
    jack_set_process_callback(this->client, this->callback_process, this);


    // allocate array
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
    in  = new jack_default_audio_sample_t*[nOutputs];

    // allocate and initialize FFT arrays
    n_buffers = L / jack_get_buffer_size(this->client);

    ifft_buff = new double[L];
    for(int i=0; i<L; i++)
        ifft_buff[i]=0;

    fft_in = new double[L];
    for(int i=0; i<L; i++)
        fft_in[i]=0;

    fft_out = new fftw_complex[L_fft];
    //    for(int i=0; i<=L; i++)
    //        fftw_complex[i] = 0;


    for(int i=0; i<n_buffers; i++)
    {
        std::vector <double*> tmp;

        for(int j=0; j<nOutputs; j++)
        {
            tmp.push_back(new double[L]);
        }

        individual_outputs.push_back(tmp);
    }

    power_spectrum.resize(L_fft);

    p_fft  =  fftw_plan_dft_r2c_1d(L, fft_in,  fft_out,   FFTW_ESTIMATE);
    p_ifft =  fftw_plan_dft_c2r_1d(L, fft_out, ifft_buff, FFTW_ESTIMATE);

    jack_activate(this->client);

    // connect inputs
    jack_connect (client, "pure_data:output0", jack_port_name(input_port[0]));
    //jack_connect (client, "pure_data:output1", jack_port_name(input_port[1]));
    // connect outputs
    jack_connect (client, jack_port_name(output_port[0]), "system:playback_1");
    jack_connect (client, jack_port_name(output_port[0]), "jaaa:in_1");

    jack_connect (client, jack_port_name(output_port[1]), "system:playback_2");
    jack_connect (client, jack_port_name(output_port[1]), "jaaa:in_2");

    jack_connect (client, jack_port_name(output_port[2]), "system:playback_1");
    jack_connect (client, jack_port_name(output_port[2]), "jaaa:in_3");

    jack_connect (client, jack_port_name(output_port[3]), "system:playback_2");
    jack_connect (client, jack_port_name(output_port[3]), "jaaa:in_4");

    // run forever
    sleep (-1);
}



int Zerr::process(jack_nframes_t nframes)
{

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

    fftw_execute(p_fft);


    // get power spectrum
    for(int tmpCNT = 0;tmpCNT<L_fft; tmpCNT++)
    {
        power_spectrum[tmpCNT] = (1.0 / (2.0*(float) L)) * (fft_out[tmpCNT][0] *fft_out[tmpCNT][0]  + fft_out[tmpCNT][1]*fft_out[tmpCNT][1]);
        // cout << power_spectrum[tmpCNT] << " ";
    }
    // cout << endl;


    get_spectral_peaks(power_spectrum, spec_peaks);


    /// IFFT with individual processing and output
    for(int outCNT=0; outCNT<nOutputs; outCNT++)
    {
        // apply gain function
        for(int smpCNT = 0;smpCNT<L_fft; smpCNT++)
        {

            float gain = gaussian_lobe(smpCNT, 30.0* (1.0 + (float) outCNT), 10.0, L_fft);

            fft_out[smpCNT][0] = fft_out[smpCNT][0]*gain;
            fft_out[smpCNT][1] = fft_out[smpCNT][1]*gain;

//            cout << gain << " ";
        }
//        cout << endl;

        fftw_execute(p_ifft);

        // copy samples to individual buffer
        for(int smpCNT = 0;smpCNT<L; smpCNT++)
        {
            individual_outputs.at(ifft_index).at(outCNT)[smpCNT] = ifft_buff[smpCNT]* get_hann_sample(smpCNT,L);
            //cout << ifft_buff[smpCNT] << " ";
        }
        // cout << endl;
    }


    // delete-loop
    for(int chanCNT=0; chanCNT<nOutputs; chanCNT++)
    {
        for(int sampCNT=0; sampCNT<nframes; sampCNT++)
            out[chanCNT][sampCNT] = 0.0;
    }

    // write all input samples to output
    for(int chanCNT=0; chanCNT<nOutputs; chanCNT++)
    {

        // the overlap add
        for(int bufCNT=0; bufCNT<n_buffers; bufCNT++)
        {

            uint buf_ind  = 0;
            float outsamp = 0;

            for(int sampCNT=0; sampCNT<nframes; sampCNT++)
            {

                // the INDIVIDUAL ola index for each buffer
                buf_ind = (sampCNT + (ifft_index * nframes) - (bufCNT*nframes)  )%L;

                // outsamp = (ifft_out[bufCNT][buf_ind] / (L_fft+n_overlap)); // * get_hann_sample(buf_ind,L);
                outsamp = (individual_outputs.at(bufCNT).at(chanCNT)[buf_ind] / (L_fft+n_overlap)); // * get_hann_sample(buf_ind,L);

                out[chanCNT][sampCNT] += outsamp*0.25;

                // cout << ifft_index << " ";

                // cout << buf_ind << " ";
                // cout << outsamp << " ";
                // cout << ifft_out[bufCNT][buf_ind] << " ";

            }

            // cout << buf_ind << " ";
            // cout << endl;

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

float Zerr::get_hann_sample(int pos, int L)
{
    float val = 0.5 * (1.0 - cos( (2.0*PI* (float) pos) / (float)L) );
    return val;
}

float Zerr::get_triangular_sample(int pos, int L)
{
    float val = 0;

    if( ((float) pos) < ((float) L / 2.0))
        val  = (1.0 / ( L/2.0)) *   pos;
    else
        val =  1.0 - ((1.0 / ( L/2.0))  *   ((float)pos-((float)L/2.0)));

    return val;
}


float Zerr::gaussian_lobe(int pos, double mu, double sigma, int L)
{

    double w     = exp( -(0.5) * pow( ((float) pos - mu) /sigma,2.0) );

    return w;
}


void Zerr::get_spectral_peaks(std::vector <double> powSpec, std::vector<int> peaks)
{

    std::vector <int> maxInd;
    std::vector <float> maxVal;

    // loop over spectrum
    for(int i = 1; i < powSpec.size()-1; ++i)
    {
        // check for local peak
        if(powSpec[i-1]<powSpec[i] && powSpec[i+1]<powSpec[i])
        {
            // check if peak is high enough
            if(0.5*(powSpec[i]-powSpec[i-1] + powSpec[i]-powSpec[i+1])>0.1)
            {
                maxInd.push_back(i);
                maxVal.push_back(powSpec[i]);
                // cout << i << ' ';
            }
        }
    }

    // cout << endl;

}
