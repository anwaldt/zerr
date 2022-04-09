#ifndef FREQUENCYTRANSFORMER_H
#define FREQUENCYTRANSFORMER_H


#include<stdlib.h>
#include<vector>
#include<algorithm>
#include<math.h>
#include<iostream>
#include<unistd.h>

#include<fftw3.h>

#include"zerr.h"

class FrequencyTransformer
{
public:

    FrequencyTransformer(int L_fft);

    void apply_window();
    void run_fft();
    void run_ifft();
    void calculate_power_spectrum();

    double* fft_input();
    fftw_complex* fft_output();
    std::vector <double> power_spectrum();

private:

    int  L_input;
    int  L_fft;

    double *fft_in;

    std::vector <double> power_spec;

    fftw_complex *fft_out;

    fftw_plan p_fft, p_ifft;

};

#endif // FREQUENCYTRANSFORMER_H
