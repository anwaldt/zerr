#ifndef THREADWALK_H
#define THREADWALK_H

#include<stdlib.h>
#include<vector>
#include<algorithm>
#include<math.h>
#include<iostream>
#include<unistd.h>

#include<jack/jack.h>

#include <complex.h>

// #include<rfftw.h>

#define PI 3.14159265



class ThreadWalk
{
public:
    ThreadWalk();

private:

    /// IFFT length
    uint L = 2048;
    uint L_fft = (L / 2 + 1);

    /// \brief nChannels
    /// the number of audio channels @todo (should not be hard-coded)

    int nInputs  = 1;
    int nOutputs = 16;

    int out_index = 0;
    double sample_buffer[2] = {0.0 , 0.0};

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


};

#endif // THREADWALK_H
