#include "frequencytransformer.h"

using std::cout;
using std::endl;

FrequencyTransformer::FrequencyTransformer(int L)
{

    L_input = L;
    L_fft   = (L / 2 + 1);

    fft_in = new double[L];

    for(int i=0; i<L; i++)
        fft_in[i]=0;

    fft_out = new fftw_complex[L_fft];

    power_spec.resize(L_fft);

    p_fft   =  fftw_plan_dft_r2c_1d(L_input, fft_in,  fft_out,  FFTW_ESTIMATE);
    p_ifft  =  fftw_plan_dft_c2r_1d(L_input, fft_out, fft_in,   FFTW_ESTIMATE);

}

void FrequencyTransformer::run_fft()
{
    fftw_execute(p_fft);
}

void FrequencyTransformer::run_ifft()
{
    fftw_execute(p_ifft);
}

void FrequencyTransformer::calculate_power_spectrum()
{
   for(int tmpCNT=0; tmpCNT<L_fft; tmpCNT++)
   {
    power_spec[tmpCNT] = (1.0 / (2.0*(float) L_fft)) * (fft_out[tmpCNT][0] *fft_out[tmpCNT][0]  + fft_out[tmpCNT][1]*fft_out[tmpCNT][1]);
    // cout << power_spectrum[tmpCNT] << " ";
   }
   // cout << endl;
}

void FrequencyTransformer::apply_window()
{
    for(int tmpCNT=0; tmpCNT<L_fft; tmpCNT++)
      fft_in[tmpCNT] = fft_in[tmpCNT] * Zerr::get_hann_sample(tmpCNT,L_fft);
}

double* FrequencyTransformer::fft_input()
{
    return fft_in;
}

fftw_complex* FrequencyTransformer::fft_output()
{
    return fft_out;
}

std::vector <double> FrequencyTransformer::power_spectrum()
{
    return  power_spec;
}
