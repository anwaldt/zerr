#ifndef WEIGHTSHIFTER_H
#define WEIGHTSHIFTER_H

#include<stdlib.h>
#include<vector>
#include<algorithm>
#include<math.h>
#include<iostream>
#include<unistd.h>

#include<jack/jack.h>

#include <complex.h>

#include"../../common/zerr.h"
#include"../../common/frequencytransformer.h"

class WeightShifter
{
public:
    WeightShifter();

private:

    /// IFFT length
    uint L = 2048;
    uint L_fft = (L / 2 + 1);

    int L_hop       = 512;
    int hop_counter = 0;

    /// number of IFFT buffer for overlap add
    uint n_buffers;

    /// \brief nChannels
    /// the number of audio channels @todo (should not be hard-coded)

    int nInputs  = 1;
    int nOutputs = 16;

    float * gain_mask;

    // used for manual scaling of flux to 0...1
    float flux_normalizer = 1.0/3.0;

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
    static int callback_process(jack_nframes_t x, void* object);

    // FFTW stuff
    FrequencyTransformer *fft;

    /// index of the most recent fft/ifft buffer
    uint ifft_index = 0;

    uint n_overlap;

};

#endif //
