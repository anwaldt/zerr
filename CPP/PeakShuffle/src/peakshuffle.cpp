#include "peakshuffle.h"

using std::cout;
using std::endl;

PeakShuffle::PeakShuffle()
{

    // cout << "Starting Jack Client!" << endl;

    this->client = jack_client_open("PeakShuffle", JackNullOption, &status, NULL);

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
    in  = new jack_default_audio_sample_t*[nOutputs];

    /// allocate and initialize FFT arrays

    n_buffers = L / jack_get_buffer_size(this->client);

    ifft_buff = new double[L];
    for(int i=0; i<L; i++)
        ifft_buff[i]=0;

    fft_in = new double[L];
    for(int i=0; i<L; i++)
        fft_in[i]=0;

    fft_out = new fftw_complex[L_fft];

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



int PeakShuffle::process(jack_nframes_t nframes)
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

    if(hop_counter>=L_hop)
    {

        fftw_execute(p_fft);


        // get power spectrum
        for(int tmpCNT = 0;tmpCNT<L_fft; tmpCNT++)
        {
            power_spectrum[tmpCNT] = (1.0 / (2.0*(float) L)) * (fft_out[tmpCNT][0] *fft_out[tmpCNT][0]  + fft_out[tmpCNT][1]*fft_out[tmpCNT][1]);
            // cout << power_spectrum[tmpCNT] << " ";
        }
        //    cout << endl;

        // Get spectral peaks:
        std::vector<std::pair<float, int> > peaks = get_spectral_peaks(power_spectrum);

        int n_peaks = peaks.size();

        if(peaks.empty()==false)
        {

            // IFFT with individual processing and output
            for(int outCNT=0; outCNT<nOutputs; outCNT++)
            {

                int peakInd = (n_peaks- outCNT)%n_peaks;

                // cout << peakInd<< " - ";

                int mu = peaks[ peakInd].second;

                // cout << mu << endl;

                float sigma = gaussian_width + (1.0 / (float) L_fft) * 20.0;

                // apply gain function
                for(int smpCNT = 0;smpCNT<L_fft; smpCNT++)
                {

                    float gain =  ((float) n_peaks /(float) nOutputs) *  gaussian_lobe(smpCNT, (float) mu , sigma, L_fft);

                    // float gain = (1/(sigma*2*PI)) * gaussian_lobe(smpCNT, (float) mu * (1.0 + (float) outCNT), sigma, L_fft);

                    fft_out[smpCNT][0] = fft_out[smpCNT][0]*gain;
                    fft_out[smpCNT][1] = fft_out[smpCNT][1]*gain;

                    // cout << gain << " ";
                }

                fftw_execute(p_ifft);

                // copy samples to individual buffer
                for(int smpCNT = 0;smpCNT<L; smpCNT++)
                {
                    individual_outputs.at(ifft_index).at(outCNT)[smpCNT] = ifft_buff[smpCNT]* get_hann_sample(smpCNT,L);
                    //cout << ifft_buff[smpCNT] << " ";
                }

                // cout << endl;

            }
        }
        // if no spectral peaks are found:
        else
        {
            // fill all output buffers with "0.0":
            for(int outCNT=0; outCNT<nOutputs; outCNT++)
            {
                for(int smpCNT = 0;smpCNT<L; smpCNT++)
                {
                    individual_outputs.at(ifft_index).at(outCNT)[smpCNT] = 0.0;
                }
            }
        }

        hop_counter=0;

    }

    /// Output to all channel buffers:

    // set output buffer "0.0"
    for(int chanCNT=0; chanCNT<nOutputs; chanCNT++)
    {
        for(int sampCNT=0; sampCNT<nframes; sampCNT++)
            out[chanCNT][sampCNT] = 0.0;
    }

    // write all samples
    for(int chanCNT=0; chanCNT<nOutputs; chanCNT++)
    {

        // the overlap add loop
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

                out[chanCNT][sampCNT] += outsamp;

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

    hop_counter+=L_hop;


    return 0;
}

int PeakShuffle::callback_process(jack_nframes_t x, void* object)
{
    return static_cast<PeakShuffle*>(object)->process(x);
}

float PeakShuffle::get_hann_sample(int pos, int L)
{
    float val = 0.5 * (1.0 - cos( (2.0*PI* (float) pos) / (float)L) );
    return val;
}

float PeakShuffle::get_triangular_sample(int pos, int L)
{
    float val = 0;

    if( ((float) pos) < ((float) L / 2.0))
        val  = (1.0 / ( L/2.0)) *   pos;
    else
        val =  1.0 - ((1.0 / ( L/2.0))  *   ((float)pos-((float)L/2.0)));

    return val;
}


float PeakShuffle::gaussian_lobe(int pos, double mu, double sigma, int L)
{
    double w     = exp( -(0.5) * pow( ((float) pos - mu) /sigma,2.0) );
    return w;
}


std::vector<std::pair<float, int> > PeakShuffle::get_spectral_peaks(std::vector <double> powSpec)
{

    /// for storing local maxima with height and index
    std::vector<std::pair<float, int> > sorter;

    int last_peak = -100;

    // loop over spectrum
    for(int i = 1; i < powSpec.size()-1; ++i)
    {
        // check for local peak
        if((powSpec[i-1]<powSpec[i]) && (powSpec[i+1]<powSpec[i]))
        {

            double height = 0.5*((powSpec[i]-powSpec[i-1]) +  (powSpec[i]-powSpec[i+1]));

            if(height > min_peak_height && i>(last_peak+min_peak_distance))
            {
                sorter.push_back(std::make_pair(height, i));
                last_peak=i;
            }
        }
    }

    //    std::sort(sorter.begin(), sorter.end());

    //        for (  const std::pair<float,int> &e : sorter ){
    //            cout << e.second << ' ';
    //        }

    //        cout << endl;

    return sorter;
}
