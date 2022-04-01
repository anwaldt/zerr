
namespace Zerr {

  float gaussian_lobe(int pos, double mu, double sigma, int L)
  {
      double w     = exp( -(0.5) * pow( ((float) pos - mu) /sigma,2.0) );
      return w;
  }

}
