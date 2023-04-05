#ifndef PIXELIZE_H
#define PIXELIZE_H

#include<stdlib.h>
#include<vector>
#include<algorithm>
#include<math.h>
#include<iostream>
#include<unistd.h>

#include<jack/jack.h>


#include <complex.h>
#include <fftw3.h>

// #include<rfftw.h>

#define PI 3.14159265



class Pixelize
{
public:
    Pixelize();

private:

    /// IFFT length
    uint L = 2048;
    uint L_fft = (L / 2 + 1);

    int L_hop       = 512;
    int hop_counter = 0;

    /// number of IFFT buffer for overlap add
    uint n_buffers;

    ///
    /// \brief nChannels
    /// the number of audio channels @todo (should not be hard-coded)

    int nInputs  = 1;
    int nOutputs = 8;

    float * gain_mask;

    // used for manual scaling of flux to 0...1
    float flux_normalizer = 1.0/3.0;

    ///
    /// \brief client
    /// the jack client, obviously
    jack_client_t   *client;

    /// \brief status
    /// gets the status from the jack server
    jack_status_t   status;

    /// \brief input_port
    /// the jack input ports
    jack_port_t     **input_port;

    /// \brief output_port
    /// the jack output ports
    jack_port_t     **output_port;

    /// \brief in
    ///
    /// \brief out
    jack_default_audio_sample_t **in, **out;

    /// \brief process
    /// \param nframes
    /// \return
    ///
    int process (jack_nframes_t nframes);

    /// \brief callback_process
    ///         is used to access the members of this
    ///         class in the static mode
    /// \param x number of samples in the buffer
    /// \param object void pointer
    /// \return
    ///
    static int callback_process(jack_nframes_t x, void* object);


    // FFTW stuff


    double *fft_in;


    std::vector <double> power_spectrum;
    std::vector <double> last_power_spectrum;

    float spec_max      = 0.0;
    float last_spec_max = 0.0;


    fftw_complex *fft_out;

    fftw_plan p_fft;

    uint fft_count = 0;


    float get_hann_sample(int pos, int L);
    float get_triangular_sample(int pos, int L);

    /// index of the most recent fft/ifft buffer
    uint ifft_index = 0;

    uint n_overlap;

    float gaussian_lobe(int pos, double mu, double sigma, int L);

    std::vector<std::pair<float, int> > get_spectral_peaks(std::vector <double> powSpec);

};

#endif // ZERR_H
